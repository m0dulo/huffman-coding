#ifndef HUFFMANDEMO_H
#define HUFFMANDEMO_H

#include <QMainWindow>
#include "ui_HuffmanDemo.h"

class Huffman;

class HuffmanDemo : public QMainWindow
{
    Q_OBJECT

public:
    HuffmanDemo(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~HuffmanDemo();

private:
    bool isOriginalFile(const QString& fileName);
    bool isCompressedFile(const QString& fileName);
    bool getOutputFileByInputFile(const QString& inputFile,QString& outputFile);

private slots:
    void selectInputFileSlot();
    void selectOutputFileSlot();
    void CodeSlot();
    void DecodeSlot();
    void updateUiSlot(const QString & text);
    void log(const QString& log);

private:
    Ui::HuffmanDemoClass ui;
    QString m_InputFile;
    QString m_OutputFile;
    Huffman* m_pHuffman;
};

#endif // HUFFMANDEMO_H
