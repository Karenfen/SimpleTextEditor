#include "filer.h"


filer::filer(QWidget *parent, const QString& filter) :
    QWidget(parent), ui(new Ui_Form)
{
    ui->setupUi(this);

    ui->comboBoxType->addItem(filter);
    ui->comboBoxType->setItemText(0, filter);

    m_model = std::make_shared<QDirModel>(this);
    ui->treeView->setModel(m_model.get());

    ui->treeView->setColumnWidth(0, 400);
    resize(860, 480);
    setWindowModality(Qt::WindowModality::ApplicationModal);
    setWindowIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    setWindowTitle(tr("Проводник"));

    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &filer::selectionChanged);
}

filer::~filer()
{
    delete ui;
}

void filer::refresh()
{
    m_model->refresh();
}

void filer::retranslateUi()
{
    ui->retranslateUi(this);
}


void filer::on_quit_clicked()
{
    close();
    ui->fileName->clear();
}

void filer::selectionChanged()
{
    if(ui->treeView->selectionModel()->hasSelection())
    {
        QModelIndex index = ui->treeView->selectionModel()->currentIndex();
        QString path = m_model->filePath(index);
        ui->currentPath->setText(path);
    }
}




void filer::on_open_clicked()
{
    QString path;
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();

    if(!m_model->isDir(index))
    {
        path = ui->currentPath->text();
    }
    else
    {
        path = ui->currentPath->text() + "/" + ui->fileName->text() + ui->comboBoxType->currentText();
    }

    emit fileSelected(path);
    close();
    ui->fileName->clear();
}


void filer::on_setPath_clicked()
{
    // пока не понял как это реализовать
}

