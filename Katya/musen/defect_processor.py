from tqdm import tqdm
import argparse
from pathlib import Path


def load_particles(particle_filename: str):
    particles = {} # key: id value: (radius, [positions])

    time_marker = '2'
    radius_marker = '5'
    coord_marker = '12'
    events = -1

    with open(particle_filename, 'r') as f:
        lines = f.readlines()
        for idx, l in tqdm(enumerate(lines), desc='Reading particle file', total=len(lines)):
            data = l.split(' ')
            if data[0] == '\n':
                continue
            if data[0] != '0':
                raise RuntimeError(f'Line {idx}: First column is expected to be zero, got {data[0]}')
            id = int(data[1])
            radius = -1
            positions = [] # key -- time point, value -- (x, y, z)
            idx = 2
            while idx < len(data):
                if data[idx] == radius_marker:
                    radius = float(data[idx + 1])
                    idx += 2
                elif data[idx] == time_marker:
                    time = float(data[idx + 1])
                    if data[idx + 2] != coord_marker:
                        raise RuntimeError(f'Line {idx}: {coord_marker} is expected after <{time_marker} tp>. Got {data[idx + 2]}')
                    pos = (float(data[idx + 3]) * 1000, float(data[idx + 4]) * 1000, float(data[idx + 5]) * 1000)
                    idx += 6
                    positions.append((time, pos))
                else:
                    idx += 1
            if events != -1:
                if len(positions) != events:
                    raise RuntimeError(f'Line {idx}: Different particles have different events count')
            else:
                events = len(positions)
            if radius == -1:
                raise RuntimeError(f'Line {idx}: Radius was not mentioned')
            particles[id] = (radius, positions)
    return particles, events


def load_bonds(bond_filename: str):
    # key: id value: [begin_particle_id, end_particle_id, death tp, {time: (x, y, z)}, mtl]
    bonds = {}
    
    time_marker = '2'
    ends_marker = '5'
    mtl_marker = '23'
    ttl_marker = '24'
    coord_marker = '12'

    events = -1
    has_all_coords = True
    has_coords = True
    with open(bond_filename, 'r') as f, open('defect_processor_log.txt', 'w') as log:
        lines = f.readlines()
        for l in tqdm(lines, desc='Reading bond file', total=len(lines)):
            data = l.split(' ')
            if data[0] == '\n':
                continue
            if data[0] != '0':
                raise RuntimeError(f'Line {idx}: First column is expected to be zero, got {data[0]}')
            id = int(data[1])
            begin_id = -1
            end_id = -1
            death_tp = -1
            idx = 2
            mtl = None
            x = None
            y = None
            z = None
            time = None
            times = {} # key: time value: (x, y, z)
            cur_events = 0
            while idx < len(data):
                if data[idx] == ends_marker:
                    begin_id, end_id = int(data[idx + 1]), int(data[idx + 2])
                    idx += 3
                elif data[idx] == ttl_marker:
                    start_tp, death_tp = float(data[idx + 1]), float(data[idx + 2])
                    if start_tp != 0:
                        raise RuntimeError(f'Line {idx}: Particle was born unexpected: born time is {start_tp}, expected 0')
                    idx += 3
                elif data[idx] == time_marker:
                    time = float(data[idx + 1])
                    cur_events += 1
                    idx += 2
                elif data[idx] == coord_marker:
                    x, y, z = float(data[idx + 1]), float(data[idx + 2]), float(data[idx + 3])
                    # suppose coordinates appear after time
                    times[time] = (x * 1000, y * 1000, z * 1000)
                    idx += 4
                elif data[idx] == mtl_marker:
                    mtl = data[idx + 1]
                    idx += 2
                else:
                    idx += 1
            if begin_id == -1 or end_id == -1 or death_tp == -1:
                raise RuntimeError(f'Line {idx}: Expected begin and end particle id, death time point. Got {begin_id}, {end_id}, {death_tp}')
            if events != -1:
                if cur_events != events:
                    raise RuntimeError(f'Line {idx}: Different particles have different events count')
            else:
                events = cur_events
            has_coords = len(times) > 0
            has_all_coords &= has_coords
            bonds[id] = [begin_id, end_id, death_tp, times if has_coords else None, mtl]
    return bonds, {'events': events, 'has_all_coords': has_all_coords}


def is_particle_in_ROI(radius: float, height: float, bond_length: float, 
                        x: float, y: float, z: float) -> bool:
    if not (-(height + bond_length) <= z < height + bond_length):
        return False
    if x * x + y * y > (radius + bond_length) ** 2:
        return False
    return True


def binary_search(arr, elem):
    total = len(arr)
    l = 0
    r = total

    while r - l > 1:
        cur = (l + r) // 2
        if elem < arr[cur]:
            r = cur
        elif elem > arr[cur]:
            l = cur
        else:
            return cur, arr[cur]
    return l, arr[l]


def list_to_str(l: list) -> list[str]:
    for i in range(len(l)):
        l[i] = str(l[i])
    return l


def process_particles_bonds(particles: dict, bonds: dict, radius: float,
                            height: float, bond_length: float, out_filename: str):
    sep = ' '
    # add death time to all particles if they flew out of sample
    particle_death = {id: 2 ** 100 for id in particles}
    for id, particle in tqdm(particles.items(), desc='Evaluate particle death time. Step #1', total=len(particles)):
        rad, positions = particle
        prev_time = 0
        for time, position in positions:
            if not is_particle_in_ROI(radius, height, bond_length, *position):
                particle_death[id] = prev_time
                break
            prev_time = time

    # re-evaluate death time of bonds according to death times of particles
    for id, bond in tqdm(bonds.items(), desc='Evaluate particle death time. Step #2',total=len(bonds)):
        begin_id, end_id, death_tp = bond[0], bond[1], bond[2]
        # assign new death time as minimum of all death times
        bond[2] = min(particle_death[begin_id], particle_death[end_id], death_tp)

    with open(out_filename, 'w') as f, open(str(Path(out_filename).parent /'time_points.txt'), 'w') as tpf:
        time_points = []
        particle0 = particles[next(iter(particles))][1]
        for tp, position in particle0:
            time_points.append(tp)
        time_points.sort()

        for tp in time_points:
            tpf.write(f'{tp} ')
        tpf.write('\n')

        f.write(sep.join(['BondId', 'BondX,mm', 'BondY,mm', 'BondZ,mm', 'DeathT,s',
                'BeginParticleId', 'BeginParticleX,mm', 'BeginParticleY,mm', 'BeginParticleZ,mm',
                'EndParticleId', 'EndParticleX,mm', 'EndParticleY,mm', 'EndParticleZ,mm']) + '\n')

        for id, bond in tqdm(bonds.items(), desc='Save result to file', total=len(bonds)):
            begin_id, end_id, death_tp = bond[0], bond[1], bond[2]

            if death_tp > time_points[-1] - 1e-5:
                continue

            tp_idx = binary_search(time_points, death_tp)[0]

            begin_radius, begin_positions = particles[begin_id]
            begin_coord = begin_positions[tp_idx][1]

            end_radius, end_positions = particles[end_id]
            end_coord = end_positions[tp_idx][1]

            middle_coord = tuple((begin_coord[i] + end_coord[i]) / 2 for i in range(3))
            
            output = sep.join(list_to_str([id, *middle_coord, death_tp, begin_id, *begin_coord, end_id, *end_coord]))
            f.write(output + '\n')


def process_bonds(bonds: dict, radius: float, height: float, bond_length: float, out_filename: str):
    sep = ' '
    with open(out_filename, 'w') as f, open(str(Path(out_filename).parent /'time_points.txt'), 'w') as tpf:
        time_points = []
        bonds0 = bonds[next(iter(bonds))][3]
        for tp in bonds0:
            time_points.append(tp)
        time_points.sort()

        for tp in time_points:
            tpf.write(f'{tp} ')
        tpf.write('\n')

        f.write(sep.join(['BondId', 'BondX,mm', 'BondY,mm', 'BondZ,mm', 'DeathT,s',
                'BeginParticleId', 'EndParticleId', 'Material']) + '\n')

        for id, bond in tqdm(bonds.items(), desc='Save result to file', total=len(bonds)):
            begin_id, end_id, death_tp, time_to_coord, mtl = bond[0], bond[1], bond[2], bond[3], bond[4]

            if death_tp > time_points[-1] - 1e-5:
                continue

            tp = binary_search(time_points, death_tp)[1]
            coord = time_to_coord[tp]
            if is_particle_in_ROI(radius, height, bond_length, *coord):
                output = sep.join(list_to_str([id, *coord, death_tp, begin_id, end_id, mtl if mtl is not None else '']))
                f.write(output + '\n')


def main():
    parser = argparse.ArgumentParser(description='Musen defect processor')

    parser.add_argument('--particle', '-p', default='particles.txt', type=str,
                        help='Particle file')
    parser.add_argument('--bond', '-b', default='bonds.txt', type=str,
                        help='Bonds file')
    parser.add_argument('--compute_coords', '-c', action='store_true',
                        help='Compute bond coordinates with own functionality')
    parser.add_argument('--radius', '-r', default=5.0, type=float,
                        help='Sample radius')
    parser.add_argument('--height', default=20.0, type=float,
                        help='Sample height')
    parser.add_argument('--length', '-l', default=0.3, type=float,
                        help='Average bond length')
    parser.add_argument('--out', '-o', default='bonds_preprocessed.txt', type=str,
                        help='Output file')                        

    args = parser.parse_args()
    try:
        if args.compute_coords:
            particles, particles_events = load_particles(args.particle)
            bonds, bonds_events = load_bonds(args.bond)

            if particles_events != bonds_events['events']:
                raise RuntimeError(f'Bond and particles files are mismatched. '\
                                f'Got {particles_events} particles events, {bonds_events} bonds events')
            process_particles_bonds(particles, bonds, args.radius, args.height, args.length, args.out)
        else:
            print('WARNING: particles file is not needed in this mode')
            bonds, bonds_events = load_bonds(args.bond)
            if not bonds_events['has_all_coords']:
                raise RuntimeError('Some bonds do not have coords')
            process_bonds(bonds, args.radius, args.height, args.length, args.out)
    except Exception as e:
        print(f'Exception occured: {e}')


if __name__ == '__main__':
    main()
