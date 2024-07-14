import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QTextEdit, QVBoxLayout, QWidget, QPushButton, QFileDialog, \
    QHBoxLayout


class PoetryReader(QMainWindow):
    def __init__(self):
        super().__init__()
        self.initUI()
        self.fileName = None

    def initUI(self):
        self.setGeometry(100, 100, 600, 400)

        self.textEdit = QTextEdit(self)
        self.openButton = QPushButton('Open', self)
        self.openButton.clicked.connect(self.openFile)
        self.saveButton = QPushButton('Save', self)
        self.saveButton.clicked.connect(self.saveFile)

        buttonLayout = QHBoxLayout()
        buttonLayout.addWidget(self.openButton)
        buttonLayout.addWidget(self.saveButton)

        layout = QVBoxLayout()
        layout.addWidget(self.textEdit)
        layout.addLayout(buttonLayout)

        centralWidget = QWidget(self)
        centralWidget.setLayout(layout)
        self.setCentralWidget(centralWidget)

    def openFile(self):
        options = QFileDialog.Options()
        fileName, _ = QFileDialog.getOpenFileName(self, "选择一个文件", "", "All Files (*)",
                                                  options=options)
        if fileName:
            with open(fileName, 'r', encoding='utf-8') as file:
                content = file.read()
                self.textEdit.setText(content)
                self.fileName = fileName

    def saveFile(self):
        if self.fileName:
            with open(self.fileName, 'w', encoding='utf-8') as file:
                content = self.textEdit.toPlainText()
                file.write(content)
        else:
            options = QFileDialog.Options()
            fileName, _ = QFileDialog.getSaveFileName(self, "保存文件", "", "All Files (*)",
                                                      options=options)
            if fileName:
                with open(fileName, 'w', encoding='utf-8') as file:
                    content = self.textEdit.toPlainText()
                    file.write(content)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    reader = PoetryReader()
    reader.show()
    sys.exit(app.exec_())
