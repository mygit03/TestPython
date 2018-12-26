#include "widget.h"

#include <QGridLayout>
#include <QFileDialog>
//#include <Python.h>
#include <iostream>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("调用Python脚本实现简单的四则运算"));
    init();
}

Widget::~Widget()
{
}

void Widget::init()
{
    QGridLayout *gridLayout = new QGridLayout(this);
    m_pFileLabel = new QLabel(tr("Python File:"), this);
    gridLayout->addWidget(m_pFileLabel, 0, 0, 1, 1);

    m_pFileLE = new QLineEdit(this);
    m_pFileLE->setReadOnly(true);
    gridLayout->addWidget(m_pFileLE, 0, 1, 1, 5);

    m_pFileBtn = new QPushButton(tr("选择"), this);
    connect(m_pFileBtn, SIGNAL(clicked(bool)), this, SLOT(choosePythonFile()));
    gridLayout->addWidget(m_pFileBtn, 0, 6, 1, 1);

    m_pALabel = new QLabel(tr("参数a:"), this);
    m_pALabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    gridLayout->addWidget(m_pALabel, 1, 0, 1, 1);

    m_pADoubleSpinBox = new QDoubleSpinBox(this);
    m_pADoubleSpinBox->setToolTip(tr("参数a"));
    gridLayout->addWidget(m_pADoubleSpinBox, 1, 1, 1, 1);

    m_pBLabel = new QLabel(tr("参数b:"), this);
    m_pBLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    gridLayout->addWidget(m_pBLabel, 1, 2, 1, 1);

    m_pBDoubleSpinBox = new QDoubleSpinBox(this);
    m_pBDoubleSpinBox->setToolTip(tr("参数b"));
    gridLayout->addWidget(m_pBDoubleSpinBox, 1, 3, 1, 1);

    m_pCalculateBtn = new QPushButton(tr("计算"), this);
    connect(m_pCalculateBtn, SIGNAL(clicked(bool)), this, SLOT(calculate()));
    gridLayout->addWidget(m_pCalculateBtn, 1, 6, 1, 1);

    m_pSumLabel = new QLabel(tr("Sum:"), this);
    m_pSumLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    gridLayout->addWidget(m_pSumLabel, 2, 0, 1, 1);

    m_pSumLE = new QLineEdit(this);
    m_pSumLE->setReadOnly(true);
    gridLayout->addWidget(m_pSumLE, 2, 1, 1, 1);

    m_pSubLabel = new QLabel(tr("Sub:"), this);
    m_pSubLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    gridLayout->addWidget(m_pSubLabel, 2, 2, 1, 1);

    m_pSubLE = new QLineEdit(this);
    m_pSubLE->setReadOnly(true);
    gridLayout->addWidget(m_pSubLE, 2, 3, 1, 1);

    m_pMulLabel = new QLabel(tr("Mul:"), this);
    m_pMulLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    gridLayout->addWidget(m_pMulLabel, 2, 4, 1, 1);

    m_pMulLE = new QLineEdit(this);
    m_pMulLE->setReadOnly(true);
    gridLayout->addWidget(m_pMulLE, 2, 5, 1, 1);

    m_pDivLabel = new QLabel(tr("Div:"), this);
    m_pDivLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    gridLayout->addWidget(m_pDivLabel, 3, 0, 1, 1);

    m_pDivLE = new QLineEdit(this);
    m_pDivLE->setReadOnly(true);
    gridLayout->addWidget(m_pDivLE, 3, 1, 1, 1);

    m_pModLabel = new QLabel(tr("Mod:"), this);
    m_pModLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    gridLayout->addWidget(m_pModLabel, 3, 2, 1, 1);

    m_pModLE = new QLineEdit(this);
    m_pModLE->setReadOnly(true);
    gridLayout->addWidget(m_pModLE, 3, 3, 1, 1);
}

void Widget::CallPython(const QString file)
{
    Py_Initialize();    // 初始化

    string chdir_cmd;
    // 将Python工作路径切换到待调用模块所在目录,一定要保证路径名的正确性
    string path = "E:/Qt/TestPython";
    if (!file.isEmpty())
    {
        path = QFileInfo(file).absolutePath().toStdString();
    }
    chdir_cmd = string("sys.path.append(\"") + path + "\")";
    const char* cstr_cmd = chdir_cmd.c_str();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(cstr_cmd);

    // 加载模块
    PyObject* moduleName = PyString_FromString("pytest"); //模块名,不是文件名(文件名不加后缀)
    PyObject* pModule = PyImport_Import(moduleName);
    if (!pModule) // 加载模块失败
    {
        cout << "[ERROR] Python get module failed." << endl;
        return;
    }
    cout << "[INFO] Python get module succeed." << endl;

    // 设置参数
    double a = m_pADoubleSpinBox->value();
    double b = m_pBDoubleSpinBox->value();

    PyObject* args = PyTuple_New(2);        // 2个参数
    PyObject* arg1 = PyInt_FromLong(a);     // 参数一设为4
    PyObject* arg2 = PyInt_FromLong(b);     // 参数二设为3
    PyTuple_SetItem(args, 0, arg1);
    PyTuple_SetItem(args, 1, arg2);

#if 0
    // 加载函数
    PyObject* pv = PyObject_GetAttrString(pModule, "sum");
    if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
    {
        cout << "[ERROR] Can't find funftion (sum)" << endl;
        return;
    }
    cout << "[INFO] Get function (add) succeed." << endl;

    // 调用函数
    PyObject* pRet = PyObject_CallObject(pv, args);

    // 获取结果
    if (pRet)  // 验证是否调用成功
    {
        long result = PyInt_AsLong(pRet);
    }
#endif

    m_pModule = pModule;
    m_pArgs = args;

    CalulateMethod("sum", 0);
    CalulateMethod("sub", 1);
    CalulateMethod("mul", 2);
    CalulateMethod("div", 3);
    CalulateMethod("mod", 4);

    Py_Finalize();
}

void Widget::CalulateMethod(const char *method, int type)
{
    // 加载函数
    PyObject* pv = PyObject_GetAttrString(m_pModule, method);
    if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
    {
        cout << "[ERROR] Can't find funftion (sum)" << endl;
        return;
    }
    cout << "[INFO] Get function (add) succeed." << endl;

    // 调用函数
    PyObject* pRet = PyObject_CallObject(pv, m_pArgs);

    // 获取结果
    long result = 0;
    if (pRet)  // 验证是否调用成功
    {
        result = PyInt_AsLong(pRet);
    }

    // 显示结果
    switch (type) {
    case 0:
        m_pSumLE->setText(QString::number(result));
        break;
    case 1:
        m_pSubLE->setText(QString::number(result));
        break;
    case 2:
        m_pMulLE->setText(QString::number(result));
        break;
    case 3:
        m_pDivLE->setText(QString::number(result));
        break;
    case 4:
        m_pModLE->setText(QString::number(result));
        break;
    default:
        break;
    }
}

void Widget::choosePythonFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("选择文件"), ".", tr("Python File(*.py);;ALL FILE(*.*)"));
    if (!filename.isEmpty())
        m_pFileLE->setText(filename);
}

void Widget::calculate()
{
    QString filename = m_pFileLE->text();
    CallPython(filename);
}
