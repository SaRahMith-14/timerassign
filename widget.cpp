#include "widget.h"
#include "ui_widget.h"
#include <new2.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_cancel_clicked()
{
    this->close();
}

void Widget::on_Login_clicked()
{
    QString name,key;
    name=ui->lineEdit->text().trimmed();
    key=ui->lineEdit_2->text().trimmed();
    if(name=="admin"&&key=="1234")
    {
        this->hide();
        new2 *c=new new2;
        c->show();
    }
    else{
        ui->warningLable->setText("Account or password error");
    }

}
