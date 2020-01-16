import sys
# GUI import
from Soliter import *
from NewGameDiolog import *
from PyQt5 import QtWidgets, QtGui, QtCore
from PyQt5.QtWidgets import (QWidget, QGridLayout, QPushButton, QApplication)
import random

class FieldWindow(QtWidgets.QMainWindow):
    def getNeighboorhood(self, y, x):
        return [[y - 1, x - 1], [y, x - 1], [y + 1, x - 1],
              [y - 1, x], [y + 1, x],
              [y - 1, x + 1], [y, x + 1], [y + 1, x + 1]]


    def __init__(self, Parent=None):
        super(FieldWindow, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.setWindowTitle('Сапёр')
        self.NewGameWindow = None

        self.field = None
        self.fH = None
        self.fW = None
        self.fM = None
        self.clickedButtons = None

        self.ui.action_2.triggered.connect(self.createGame)

    def createGame(self):
        if not self.NewGameWindow:
            self.setEnabled(False)
            self.NewGameWindow = NewGame(self)
            self.NewGameWindow.show()
            self.NewGameWindow.exec_()

    def createField(self, fieldW, fieldH):
        if int(self.fW) <= 50 and int(self.fH) <= 30:
            s = 30
        elif 50 < int(self.fW) <= 75 or 30 < int(self.fH) <= 45:
            s = 20
        else:
            s = 10
        for i in reversed(range(self.ui.gridLayout.count())):
            widgetToRemove = self.ui.gridLayout.itemAt(i).widget()
            widgetToRemove.setParent(None)
            widgetToRemove.deleteLater()
        for y in range(int(fieldH)):
            for x in range(int(fieldW)):
                button = QPushButton('', self)
                button.clicked.connect(self.clickButton)
                button.stat = '0'
                button.setFixedHeight(s)
                button.setFixedWidth(s)
                self.ui.gridLayout.addWidget(button, y, x)

    def initializeCells(self, width, height, m):
        self.clickedButtons = int(m)

        width = int(width)
        height = int(height)
        m = int(m)
        field = [0] * height
        for i in range(height):
            field[i] = [0] * width
        i = 1

        while 0 <= i <= m:
            y = round(random.uniform(0, height - 1))
            x = round(random.uniform(0, width - 1))
            if field[y][x] != 'm':
                field[y][x] = 'm'
            else:
                i -= 1
            i += 1
        for y in range(0, height):
            for x in range(0, width):
                if field[y][x] != 'm':
                    neighbours = self.getNeighboorhood(y, x)
                    dz = 0
                    for j in range(0, 8):
                        if neighbours[j][0] in range(0, height) and \
                           neighbours[j][1] in range(0, width) and \
                           field[neighbours[j][0]][neighbours[j][1]] == 'm':
                            dz += 1
                        field[y][x] = str(dz)
        return field

    def clickButton(self):
        button = self.sender()
        idx = self.ui.gridLayout.indexOf(button)
        location = self.ui.gridLayout.getItemPosition(idx)
        y = location[0]
        x = location[1]
        i = self.field[y][x]
        if button.stat == '0':
            if i == 'm':
                self.openButton(button, y, x, i)
                self.createGame()
            else:
                self.openButton(button, y, x, i)

    def autoPushOfZero(self, y, x):

        w = int(self.fW)
        h = int(self.fH)

        neighbours = self.getNeighboorhood(y, x)

        for j in range(8):
            if 0 <= neighbours[j][0] < h and \
               0 <= neighbours[j][1] < w:
                ind = neighbours[j][0] * w + neighbours[j][1]
                z_button = self.ui.gridLayout.itemAt(ind).widget()
                if z_button.stat == '0':
                    k = self.field[neighbours[j][0]][neighbours[j][1]]
                    self.openButton(z_button, neighbours[j][0], neighbours[j][1], k)

    def openButton(self, button, y, x, i):
        self.clickedButtons += 1
        button.setText(str(i))
        button.stat = '1'
        button.setEnabled(False)
        if i == '0':
            self.autoPushOfZero(y, x)
        self.winTheGame(self.fW, self.fH)

    def winTheGame(self, w, h):
        w = int(w)
        h = int(h)
        n = w * h
        if self.clickedButtons == n:
            self.createGame()

    def adaptiveResize(self):
        if int(self.fW) <= 50 and int(self.fH) <= 30:
            s = 30
        elif 50 < int(self.fW) <= 75 or 30 < int(self.fH) <= 45:
            s = 20
        else:
            s = 10
        w = (int(self.fW) + 1) * s + 5
        h = (int(self.fH) + 1) * s + 50
        self.resize(w, h)


class NewGame(QtWidgets.QDialog):
    def __init__(self, parent):
        super(NewGame, self).__init__()
        self.ui = Ui_Dialog()
        self.ui.setupUi(self)
        self.mainWindow = parent

        self.ui.buttonBox.accepted.connect(self.AcceptedMethod)
        self.ui.buttonBox.rejected.connect(self.RejectedMethod)

    def AcceptedMethod(self):
        h = self.ui.lineEdit.text()
        w = self.ui.lineEdit_2.text()
        m = self.ui.lineEdit_3.text()
        self.mainWindow.fW = w
        self.mainWindow.fH = h
        self.mainWindow.fM = m
        self.mainWindow.createField(w, h)
        self.mainWindow.field = self.mainWindow.initializeCells(w, h, m)
        self.mainWindow.adaptiveResize()
        self.mainWindow.setEnabled(True)

        self.mainWindow.NewGameWindow = None
        self.close()

    def RejectedMethod(self):
        self.mainWindow.NewGameWindow = None
        self.mainWindow.setEnabled(True)
        self.close()


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    application = FieldWindow()
    application.show()
    sys.exit(app.exec_())
