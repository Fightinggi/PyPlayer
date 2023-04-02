## 一、配置python库环境
在C++调用python中 学习过

1. include  同时把PC\pyconfig.h  放到include中
2. PCbuild\win32\python37.dll
3. PCbuild\win32\python37.lib
4. Lib 

这四个文件放到执行项目目录下
![image.png](https://cdn.nlark.com/yuque/0/2023/png/26699529/1679473971842-b7465063-0b9d-4da8-bba8-b897a1ea5776.png#averageHue=%23ececea&clientId=u4fb09911-6a21-4&from=paste&height=726&id=uf16ca37c&name=image.png&originHeight=726&originWidth=1816&originalType=binary&ratio=1&rotation=0&showTitle=false&size=184571&status=done&style=none&taskId=u20797ffb-6da2-4602-9977-d10efc50a32&title=&width=1816)
![image.png](https://cdn.nlark.com/yuque/0/2023/png/26699529/1679971555629-7373480f-1fd7-442b-9075-991f18c6fc31.png#averageHue=%23e0c490&clientId=u55340009-481b-4&from=paste&height=345&id=u44771222&name=image.png&originHeight=345&originWidth=637&originalType=binary&ratio=1&rotation=0&showTitle=false&size=35643&status=done&style=none&taskId=ud752d398-65d7-47a9-bb1e-3e329124d4a&title=&width=637)
编辑器项目属性：
![image.png](https://cdn.nlark.com/yuque/0/2023/png/26699529/1679971783278-7d017ec9-2510-4810-bd1f-66dd5fb64555.png#averageHue=%23f5f4f4&clientId=u02e871d5-c0ae-4&from=paste&height=716&id=u377335ff&name=image.png&originHeight=716&originWidth=1268&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68715&status=done&style=none&taskId=uc5363649-af0b-4b07-b885-25fdaa53ca8&title=&width=1268)

从python脚本里面读取配置
Qt按钮事件的Open槽函数调用python的open函数，

1. 在PyPlayer::PyPlayer构造函数中完成窗口大小和标题的初始化  和 开放接口函数的声明
   1. //初始化 根目录
   2. //载入模块  pyqt.py
   3. //获取python   pyqt.py配置项conf 
   4. 开放选择文件的接口给python  PyModule_AddFunctions
2. 定义信号槽函数  
   1. public slots：
      1. void Open();
3. 实现槽函数的功能
   1. void PyPlayer::Open()
4. 槽函数调用python的Open函数
   1. PyObject_CallObject(open, 0);//调用python的Open函数
5. python的Open函数调用c++的接口函数OpenDialog获取filename

def open():
    print(" PyPlayer Open ")
    filename = OpenDialog()
    print(filename)

6. 定义OpenDialog接口函数
   1. PyObject* OpenDialog(PyObject* self)
7. 传递接口函数给python脚本

static PyMethodDef cfuns[] = {
        {"OpenDialog", (PyCFunction)OpenDialog, METH_NOARGS, 0}
        ,{NULL} };
  	int re = PyModule_AddFunctions(pMoudle, cfuns);






