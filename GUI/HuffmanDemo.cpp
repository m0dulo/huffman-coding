#include "HuffmanDemo.h"
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include "Huffman.h"

HuffmanDemo::HuffmanDemo(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);

    m_pHuffman = new Huffman(this);

    ui.m_pCodePb->setEnabled(false);
    ui.m_pDecodePb->setEnabled(false);

    ui.m_pInputFileLe->setFocusPolicy(Qt::NoFocus);
    ui.m_pOutFileLe->setFocusPolicy(Qt::NoFocus);

    ui.m_pProgressBar->setValue(0);

    connect(ui.m_pSelectInFilePb,SIGNAL(clicked()),this,SLOT(selectInputFileSlot()));
    connect(ui.m_pSelectOutFilePb,SIGNAL(clicked()),this,SLOT(selectOutputFileSlot()));
    connect(ui.m_pCodePb,SIGNAL(clicked()),this,SLOT(CodeSlot()));
    connect(ui.m_pDecodePb,SIGNAL(clicked()),this,SLOT(DecodeSlot()));
    connect(ui.m_pInputFileLe,SIGNAL(textChanged(const QString&)),this,SLOT(updateUiSlot(const QString&)));
    connect(m_pHuffman,SIGNAL(logSignal(const QString&)),this,SLOT(log(const QString&)));
    connect(m_pHuffman, SIGNAL(progressSignal(int)), ui.m_pProgressBar, SLOT(setValue(int)));
}

HuffmanDemo::~HuffmanDemo()
{

}
//code
void HuffmanDemo::CodeSlot()
{
    qDebug()<<QString("In CodeSlot");

    log("");
    log("");
    log("----------------------Start Code-------------------------------");
    log(QString("Input file:[%1]").arg(m_InputFile));
    log(QString("Output file:[%1]").arg(m_OutputFile));

    m_pHuffman->setOutputFile(m_OutputFile);
    m_pHuffman->HuffmanCode(m_InputFile.toStdString().c_str());


    log("----------------------End Code-------------------------------");
}
//decode
void HuffmanDemo::DecodeSlot()
{
    qDebug()<<QString("In DecodeSlot");
    log("");
    log("");
    log("----------------------Start Decode-------------------------------");
    log(QString("Input file:[%1]").arg(m_InputFile));
    log(QString("Output file:[%1]").arg(m_OutputFile));

    m_pHuffman->setOutputFile(m_OutputFile);
    m_pHuffman->HuffmanDecode(m_InputFile.toStdString().c_str());

    log("----------------------End Decode-------------------------------");
}


bool HuffmanDemo::isOriginalFile(const QString& fileName)
{
    if (fileName.contains(ORIGINAL_FILE_SUFFIX))
    {
        return true;
    }

    return false;
}

bool HuffmanDemo::isCompressedFile(const QString& fileName)
{
    if (fileName.contains(COMPRESSED_FILE_SUFFIX))
    {
        return true;
    }

    return false;
}

bool HuffmanDemo::getOutputFileByInputFile(const QString& inputFile,QString& outputFile)
{
    if (isOriginalFile(inputFile))
    {
        outputFile = inputFile;
        outputFile.replace(ORIGINAL_FILE_SUFFIX,COMPRESSED_FILE_SUFFIX);

        return true;
    }
    else if (isCompressedFile(inputFile))
    {
        outputFile = inputFile;
        outputFile.replace(COMPRESSED_FILE_SUFFIX,QString("_dec%1").arg(ORIGINAL_FILE_SUFFIX));

        return true;
    }

    return false;
}

void HuffmanDemo::log(const QString& log)
{
    qDebug()<<QString("In HuffmanDemo::log");
    if (log.isEmpty())
    {
        ui.m_pLogTe->append(QString("%1").arg(log));
    }
    else
    {
        ui.m_pLogTe->append(QString("[%1]%2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(log));
    }

}

void HuffmanDemo::selectInputFileSlot()
{
    qDebug()<<QString("In selectInputFileSlot");
    QString fileName=QFileDialog::getOpenFileName(this,tr("Select File"), "/",tr("(*%1 *%2)")
        .arg(ORIGINAL_FILE_SUFFIX).arg(COMPRESSED_FILE_SUFFIX));
    if (!fileName.isEmpty())
    {
        m_InputFile = fileName;
        ui.m_pInputFileLe->setText(m_InputFile);
        ui.m_pProgressBar->setValue(0);
    }
}

void HuffmanDemo::selectOutputFileSlot()
{
    qDebug()<<QString("In selectOutputFileSlot");

    if (isOriginalFile(m_InputFile))
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/",
            QString("*%1").arg(COMPRESSED_FILE_SUFFIX));

        if (fileName.isEmpty())
        {
            return;
        }
        m_OutputFile = fileName;
        ui.m_pOutFileLe->setText(m_OutputFile);
    }
    else if (isCompressedFile(m_InputFile))
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/",
            QString("*%1").arg(ORIGINAL_FILE_SUFFIX));
        
        if (fileName.isEmpty())
        {
            return;
        }
        m_OutputFile = fileName;
        ui.m_pOutFileLe->setText(m_OutputFile);
    }
    else
    {

    }
        
}
void HuffmanDemo::updateUiSlot(const QString & text)
{
    qDebug()<<QString("In DecodeSlot:info[%1]").arg(text);

    ui.m_pCodePb->setEnabled(false);
    ui.m_pDecodePb->setEnabled(false);

    if (isOriginalFile(text))
    {
        ui.m_pCodePb->setEnabled(true);
    }
    else if (isCompressedFile(text))
    {
        ui.m_pDecodePb->setEnabled(true);
    }

    QString output;
    if (getOutputFileByInputFile(text,output))
    {
        m_OutputFile = output;
        ui.m_pOutFileLe->setText(m_OutputFile);
    }
}
