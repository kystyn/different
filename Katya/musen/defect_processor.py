from tqdm import tqdm
import argparse


def load_particles(particle_filename: str):
    particles = {} # key: id value: (radius, [positions])

    time_marker = '2'
    radius_marker = '5'
    coord_marker = '12'
    events = -1

    with open(particle_filename, 'r') as f:
        lines = f.readlines()
        for l in tqdm(lines, desc='Reading particle file', total=len(lines)):
            data = l.split(' ')
            if data[0] == '\n':
                continue
            if data[0] != '0':
                raise RuntimeError(f'First column is expected to be zero, got {data[0]}')
            id = int(data[1])
            radius = -1
            positions = {} # key -- time point, value -- (x, y, z)
            idx = 2
            while idx < len(data):
                if data[idx] == radius_marker:
                    radius = float(data[idx + 1])
                    idx += 2
                elif data[idx] == time_marker:
                    time = float(data[idx + 1])
                    if data[idx + 2] != coord_marker:
                        raise RuntimeError(f'{coord_marker} is expected after <{time_marker} tp>. Got {data[idx + 2]}')
                    pos = (float(data[idx + 3]), float(data[idx + 4]), float(data[idx + 5]))
                    idx += 6
                    positions[time] = pos
                else:
                    idx += 1
            if events != -1:
                if len(positions) != events:
                    raise RuntimeError('Different particles have different events count')
            else:
                events = len(positions)
            if radius == -1:
                raise RuntimeError('Radius was not mentioned')
            particles[id] = (radius, positions)
    return particles, events


def load_bonds(bond_filename: str):
    bonds = {} # key: id value: [begin_particle_id, end_particle_id, death tp]
    
    time_marker = '2'
    ends_marker = '5'
    ttl_marker = '24'
    time_marker = '2'
    force_marker = '18'

    events = -1
    with open(bond_filename, 'r') as f, open('defect_processor_log.txt', 'w') as log:
        lines = f.readlines()
        for l in tqdm(lines, desc='Reading bond file', total=len(lines)):
            data = l.split(' ')
            if data[0] == '\n':
                continue
            if data[0] != '0':
                raise RuntimeError(f'First column is expected to be zero, got {data[0]}')
            id = int(data[1])
            begin_id = -1
            end_id = -1
            death_tp = -1
            idx = 0
            cur_events = 0
            while idx < len(data):
                if data[idx] == ends_marker:
                    begin_id, end_id = int(data[idx + 1]), int(data[idx + 2])
                    idx += 3
                elif data[idx] == ttl_marker:
                    start_tp, death_tp = float(data[idx + 1]), float(data[idx + 2])
                    if start_tp != 0:
                        raise RuntimeError(f'Particle was born unexpected: born time is {start_tp}, expected 0')
                    idx += 3
                elif data[idx] == time_marker:
                    if data[idx + 2] != force_marker:
                        raise RuntimeError(f'Expected force marker {force_marker}, got {data[idx + 2]}')

                    time = float(data[idx + 1])
                    force = float(data[idx + 3])
                    # Suppose death tp is already known: it should be met in file before force
                    if time > death_tp:
                        if force > 1e-6:
                            log.write(f'Bond id {id}: expected zero force after death, got {force}\n')
                    cur_events += 1
                    idx += 4
                else:
                    idx += 1
            if begin_id == -1 or end_id == -1 or death_tp == -1:
                raise RuntimeError(f'Expected begin and end particle id, death time point. Got {begin_id}, {end_id}, {death_tp}')
            if events != -1:
                if cur_events != events:
                    raise RuntimeError('Different particles have different events count')
            else:
                events = cur_events
            bonds[id] = [begin_id, end_id, death_tp]
    return bonds, events


def is_particle_in_ROI(radius: float, height: float, bond_length: float, 
                        x: float, y: float, z: float) -> bool:
    if not (-(height + bond_length) <= y < height + bond_length):
        return False
    if x * x + z * z > radius + bond_length:
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
        radius, positions = particle
        prev_time = 0
        for time, position in positions.items():
            if not is_particle_in_ROI(radius, height, bond_length, *position):
                particle_death[id] = prev_time
            prev_time = time

    # re-evaluate death time of bonds according to death times of particles
    for id, bond in tqdm(bonds.items(), desc='Evaluate particle death time. Step #2',total=len(bonds)):
        begin_id, end_id, death_tp = bond[0], bond[1], bond[2]
        # assign new death time as minimum of all death times
        bond[2] = min(particle_death[begin_id], particle_death[end_id], death_tp)

    with open(out_filename, 'w') as f:
        time_points = list(particles[next(iter(particles))][1].keys())
        time_points.sort()

        f.write(sep.join(['BondId', 'BondX', 'BondY', 'BondZ', 'DeathT',
                'BeginParticleId', 'BeginParticleX', 'BeginParticleY', 'BeginParticleZ',
                'EndParticleId', 'EndParticleX', 'EndParticleY', 'EndParticleZ']) + '\n')

        for id, bond in tqdm(bonds.items(), desc='Save result to file', total=len(bonds)):
            begin_id, end_id, death_tp = bond[0], bond[1], bond[2]

            begin_radius, begin_positions = particles[begin_id]
            begin_coord = begin_positions[binary_search(time_points, death_tp)[1]]#[death_tp]

            end_radius, end_positions = particles[end_id]
            end_coord = end_positions[binary_search(time_points, death_tp)[1]]

            middle_coord = tuple((begin_coord[i] + end_coord[i]) / 2 for i in range(3))
            
            output = sep.join(list_to_str([id, *middle_coord, begin_id, *begin_coord, end_id, *end_coord]))
            f.write(output + '\n')


def main():
    parser = argparse.ArgumentParser(description='Musen defect processor')

    parser.add_argument('--particle', '-p', default='particles.txt', type=str,
                        help='Particle file')
    parser.add_argument('--bond', '-b', default='bonds.txt', type=str,
                        help='Bonds file')
    parser.add_argument('--radius', '-r', default=5.0, type=float,
                        help='Sample radius')
    parser.add_argument('--height', default=20.0, type=float,
                        help='Sample height')
    parser.add_argument('--length', '-l', default=0.3, type=float,
                        help='Average bond length')
    parser.add_argument('--out', '-o', default='out.txt', type=str,
                        help='Output file')                        

    args = parser.parse_args()
    particles, particles_events = load_particles(args.particle)
    bonds, bonds_events = load_bonds(args.bond)

    if particles_events != bonds_events:
        raise RuntimeError(f'Bond and particles files are mismatched. '\
                        f'Got {particles_events} particles events, {bonds_events} bonds events')
    process_particles_bonds(particles, bonds, args.radius, args.height, args.length, args.out)    


if __name__ == '__main__':
    main()
