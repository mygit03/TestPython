#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>

#include <Python.h>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void init();

    void CallPython(const QString file);

    void CalulateMethod(const char * method, int type = 0);

private slots:
    void choosePythonFile();

    void calculate();

private:
    QLabel          *m_pFileLabel;
    QLineEdit       *m_pFileLE;
    QPushButton     *m_pFileBtn;

    QLabel          *m_pALabel;
    QDoubleSpinBox  *m_pADoubleSpinBox;
    QLabel          *m_pBLabel;
    QDoubleSpinBox  *m_pBDoubleSpinBox;
    QPushButton     *m_pCalculateBtn;

    QLabel          *m_pSumLabel;
    QLineEdit       *m_pSumLE;
    QLabel          *m_pSubLabel;
    QLineEdit       *m_pSubLE;
    QLabel          *m_pMulLabel;
    QLineEdit       *m_pMulLE;
    QLabel          *m_pDivLabel;
    QLineEdit       *m_pDivLE;
    QLabel          *m_pModLabel;
    QLineEdit       *m_pModLE;

    PyObject        *m_pModule;
    PyObject        *m_pArgs;
};

#endif // WIDGET_H
