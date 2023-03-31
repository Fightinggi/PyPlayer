//Python��slots �� ��qt��slot��ͻ��������ǰ��
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
    // ff = PyFFmpeg()  SetImage(ff)  ����������
    PyFFmpeg* ff_obj = (PyFFmpeg*)arg;
    view->SetImageSig(ff_obj->ff->rgb);
    Py_RETURN_TRUE;
}

//���Žӿڸ�python����python�л�ȡ�ļ���ַfilename
PyObject* OpenDialog(PyObject* self)
{
    cout << "PyObject* OpenDialog" << endl;
    QString filename = "";
    filename = QFileDialog::getOpenFileName();
    if (filename.isEmpty())
        return PyUnicode_FromString("failed");
    return PyUnicode_FromString(filename.toStdString().c_str());
}


//PyPlayer��ʵ��Open()��Ա����
void PyPlayer::Open()
{
    cout << "PPyPlayer::Open" << endl;
    //����python��open����
    if (!pMoudle) return;
    //��ȡȫ����GIL
    PyGILState_STATE gil;
    gil = PyGILState_Ensure();
    // cout << "PPyPlayer::Open get Global GIL  Lock" << endl;

    PyObject* open = PyObject_GetAttrString(pMoudle, "open");
    if (!open || !PyCallable_Check(open)) {
        PyErr_Print(); //�׳��쳣
        PyGILState_Release(gil);//�ͷ�ȫ����
        return;
    }
    PyObject_CallObject(open, 0);//����python��Open����

    
    PyGILState_Release(gil);//�ͷ�ȫ����
    // cout << "PPyPlayer::Open  PyGILState_Release" << endl;
}

//ִ��main�������߳�
void run_main()
{
    //�����߳� ����python��main����
    PyObject* main_fun = PyObject_GetAttrString(pMoudle, "main");
    if (main_fun && PyCallable_Check(main_fun))
    {
        PyObject_CallObject(main_fun, 0);//����python��Open����
    }
    else
    {
        cout << " set main_fun failed" << endl;
    }
}


//PyPlayer��ʵ�ֹ��캯��
PyPlayer::PyPlayer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    view = ui.view;

    //��ʼ�� ��Ŀ¼
    Py_SetPythonHome(L"./");
    Py_Initialize();

    //����ģ��  pyqt.py
    pMoudle = PyImport_ImportModule("pyqt");  //PyImport_GetModule�൱�ڻ�ȡ��ģ��
    if (!pMoudle) {
        cout<<"PyImport_ImportModule pyqt failed" << endl;
        PyErr_Print(); //�׳��쳣
        return;
    }
    
    //��ȡpython   pyqt.py������conf 
    PyObject* conf = PyObject_GetAttrString(pMoudle, "conf");
    if (!conf) {
        cout << "PyObject_GetAttrString set conf failed" << endl;
        PyErr_Print(); //�׳��쳣
        return;
    }
    //��ȡpython���ڴ�С
    PyObject* key = PyUnicode_FromString("width");
    int width = PyLong_AsLong(PyDict_GetItem(conf, key));
    Py_XDECREF(key);

    key = PyUnicode_FromString("heigh");
    int heigh = PyLong_AsLong(PyDict_GetItem(conf, key));
    Py_XDECREF(key);

    //��ȡpython���ڱ���
    key = PyUnicode_FromString("title");
    wchar_t title[1024] = { 0 };  //title ���صõ��� -1
    int test = PyUnicode_AsWideChar(PyDict_GetItem(conf, key), title, sizeof(title));
    Py_XDECREF(key);
    Py_XDECREF(conf);
    //�ı�qt����
    if (width > 0 && heigh > 0)
    {
        resize(width, heigh);
        ui.view->resize(width, heigh);
    }
        
    this->setWindowTitle(QString::fromUtf16((char16_t*)title));
    

    //����ѡ���ļ��Ľӿڸ�python
    /////3.2���ݺ�����python�ű�
    ////  static �������������ļ���������PyPlayer::PyPlayer���캯��ִ�н�������ʧ��
    static PyMethodDef cfuns[] = {
        {"OpenDialog", (PyCFunction)OpenDialog, METH_NOARGS, 0},
        {"SetImage", (PyCFunction)SetImage, METH_O, 0} // һ������SetImage(ff)
        ,{NULL}
    };
    int re = PyModule_AddFunctions(pMoudle, cfuns);//������ģ��__main__
    if (re != 0) PyErr_Print();


    //�����߳� ����python��main����
    std::thread t1(run_main);
    t1.detach();

    //�������߳�  �Ѻ�������������main�������߳��еĻ�  ����ʾ��������  ������main����
    // ����Ҳ��һֱ��ˢ�� ���е�
    //PyObject* main_fun = PyObject_GetAttrString(pMoudle, "main");
    //if (main_fun && PyCallable_Check(main_fun))
    //{
    //    PyObject_CallObject(main_fun, 0);//����python��Open����
    //}
    //else
    //{
    //    cout << " set main_fun failed" << endl;
    //}

}

PyPlayer::~PyPlayer()
{}
