//Python的slots 宏 与qt的slot冲突，放在最前面
#include <Python.h>
#include <iostream>
#include "PyPlayer.h"
#include "PyFFmpeg.h"
#include <QFileDialog>
#include <thread>
using namespace std;
static PyObject* pMoudle = NULL;
static VideoView* view = 0;

PyObject* SetImage(PyObject* self, PyObject* arg)
{
    // ff = PyFFmpeg()  SetImage(ff)  传进来对象
    PyFFmpeg* ff_obj = (PyFFmpeg*)arg;
    view->SetImageSig(ff_obj->ff->rgb);
    Py_RETURN_TRUE;
}

//开放接口给python，从python中获取文件地址filename
PyObject* OpenDialog(PyObject* self)
{
    cout << "PyObject* OpenDialog" << endl;
    QString filename = "";
    filename = QFileDialog::getOpenFileName();
    if (filename.isEmpty())
        return PyUnicode_FromString("failed");
    return PyUnicode_FromString(filename.toStdString().c_str());
}


//PyPlayer类实现Open()成员函数
void PyPlayer::Open()
{
    cout << "PPyPlayer::Open" << endl;
    //调用python的open函数
    if (!pMoudle) return;
    //获取全局锁GIL
    PyGILState_STATE gil;
    gil = PyGILState_Ensure();
    // cout << "PPyPlayer::Open get Global GIL  Lock" << endl;

    PyObject* open = PyObject_GetAttrString(pMoudle, "open");
    if (!open || !PyCallable_Check(open)) {
        PyErr_Print(); //抛出异常
        PyGILState_Release(gil);//释放全局锁
        return;
    }
    PyObject_CallObject(open, 0);//调用python的Open函数

    
    PyGILState_Release(gil);//释放全局锁
    // cout << "PPyPlayer::Open  PyGILState_Release" << endl;
}

//执行main函数的线程
void run_main()
{
    //开启线程 调用python的main函数
    PyObject* main_fun = PyObject_GetAttrString(pMoudle, "main");
    if (main_fun && PyCallable_Check(main_fun))
    {
        PyObject_CallObject(main_fun, 0);//调用python的Open函数
    }
    else
    {
        cout << " set main_fun failed" << endl;
    }
}


//PyPlayer类实现构造函数
PyPlayer::PyPlayer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    view = ui.view;

    //初始化 根目录
    Py_SetPythonHome(L"./");
    Py_Initialize();

    //载入模块  pyqt.py
    pMoudle = PyImport_ImportModule("pyqt");  //PyImport_GetModule相当于获取主模块
    if (!pMoudle) {
        cout<<"PyImport_ImportModule pyqt failed" << endl;
        PyErr_Print(); //抛出异常
        return;
    }
    
    //获取python   pyqt.py配置项conf 
    PyObject* conf = PyObject_GetAttrString(pMoudle, "conf");
    if (!conf) {
        cout << "PyObject_GetAttrString set conf failed" << endl;
        PyErr_Print(); //抛出异常
        return;
    }
    //获取python窗口大小
    PyObject* key = PyUnicode_FromString("width");
    int width = PyLong_AsLong(PyDict_GetItem(conf, key));
    Py_XDECREF(key);

    key = PyUnicode_FromString("heigh");
    int heigh = PyLong_AsLong(PyDict_GetItem(conf, key));
    Py_XDECREF(key);

    //获取python窗口标题
    key = PyUnicode_FromString("title");
    wchar_t title[1024] = { 0 };  //title 返回得到的 -1
    int test = PyUnicode_AsWideChar(PyDict_GetItem(conf, key), title, sizeof(title));
    Py_XDECREF(key);
    Py_XDECREF(conf);
    //改变qt窗口
    if (width > 0 && heigh > 0)
    {
        resize(width, heigh);
        ui.view->resize(width, heigh);
    }
        
    this->setWindowTitle(QString::fromUtf16((char16_t*)title));
    

    //开放选择文件的接口给python
    /////3.2传递函数给python脚本
    ////  static 作用域在整个文件，不会因PyPlayer::PyPlayer构造函数执行结束而消失。
    static PyMethodDef cfuns[] = {
        {"OpenDialog", (PyCFunction)OpenDialog, METH_NOARGS, 0},
        {"SetImage", (PyCFunction)SetImage, METH_O, 0} // 一个参数SetImage(ff)
        ,{NULL}
    };
    int re = PyModule_AddFunctions(pMoudle, cfuns);//传给主模块__main__
    if (re != 0) PyErr_Print();


    //开启线程 调用python的main函数
    std::thread t1(run_main);
    t1.detach();

    //不开启线程  把含有阻塞操作的main放在主线程中的话  会显示不出界面  阻塞在main中了
    // 界面也是一直在刷新 运行的
    //PyObject* main_fun = PyObject_GetAttrString(pMoudle, "main");
    //if (main_fun && PyCallable_Check(main_fun))
    //{
    //    PyObject_CallObject(main_fun, 0);//调用python的Open函数
    //}
    //else
    //{
    //    cout << " set main_fun failed" << endl;
    //}

}

PyPlayer::~PyPlayer()
{}
