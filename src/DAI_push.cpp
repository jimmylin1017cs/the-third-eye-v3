#include "DAI_push.h"

#include "opencv2/opencv.hpp"

#define CHECK_PYTHON_NULL(p) \
    if (NULL == (p)) {\
        PyErr_Print();\
        exit(EXIT_FAILURE);\
    }


#ifdef __cplusplus
extern "C" {
#endif

static PyObject *pModule, *pDict, *pFunc;
static int first_called = 1;
static cv::Mat m, frame;

/*void iot_talk(image im, person_box *person_boxes, int person_amount)
{
    std::vector<person_box> boxes;
    //boxes.clear();

    for(int i = 0; i < person_amount; i++)
    {
        boxes.push_back(person_boxes[i]);
    }

    std::vector<unsigned char> outbuf;
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY); // default quality value is 95
    compression_params.push_back(30);

    frame = image_to_mat(im);
    //m = image_to_mat(im);
    //cv::resize(m, frame, cv::Size(800, 600), 0, 0);

    cv::imencode(".jpg", frame, outbuf, compression_params); // encodes an image into a memory buffer

    if(first_called)
    {
        first_called = 0;
        iot_init();
    }
    
    iot_send(outbuf, boxes);
}*/


void iot_init()
{
    if(!Py_IsInitialized())
    {
        Py_Initialize();
    }

    //PySys_SetPath(PYTHON_IMPORT_PATH);

    //setenv("PYTHONPATH", PYTHON_IMPORT_PATH, 1);

    pModule = PyImport_ImportModule("DAI_push");
    CHECK_PYTHON_NULL(pModule)

    pDict = PyModule_GetDict(pModule);
    CHECK_PYTHON_NULL(pDict)

    pFunc = PyDict_GetItemString(pDict, "send_boxes_to_iottalk");
    CHECK_PYTHON_NULL(pFunc)
}

void iot_talk_send(std::vector<person_sort_det> &person_sort_dets, double &timer)
{
    printf("In iot_talk_send()");
    if(first_called)
    {
        first_called = 0;
        iot_init();
    }

    int sort_dets_size = person_sort_dets.size();

    PyObject *pBoxDict[sort_dets_size]; // store all boxes information
    for(int i = 0; i < sort_dets_size; i++) pBoxDict[i] = PyDict_New();

    PyObject *pBoxList  = PyList_New(sort_dets_size); // store all box dictionaries
    
    PyObject *pArgList = PyTuple_New(1); // arguments for function

    // create python dictionary with box information
    for(int i = 0; i < sort_dets_size; i++)
    {
        PyDict_SetItem(pBoxDict[i], PyString_FromString("timer"), PyFloat_FromDouble(timer));
        PyDict_SetItem(pBoxDict[i], PyString_FromString("id"), PyInt_FromLong(person_sort_dets[i].id));
        PyDict_SetItem(pBoxDict[i], PyString_FromString("x1"), PyInt_FromLong(person_sort_dets[i].x1));
        PyDict_SetItem(pBoxDict[i], PyString_FromString("y1"), PyInt_FromLong(person_sort_dets[i].y1));
        PyDict_SetItem(pBoxDict[i], PyString_FromString("x2"), PyInt_FromLong(person_sort_dets[i].x2));
        PyDict_SetItem(pBoxDict[i], PyString_FromString("y2"), PyInt_FromLong(person_sort_dets[i].y2));
    }

    //std::cout<<"pBoxList"<<std::endl;

    // put all dictionaries into list
    for(int i = 0; i < sort_dets_size; i++)
    {
        PyList_SetItem(pBoxList, i, pBoxDict[i]);
    }

    PyTuple_SetItem(pArgList, 0, pBoxList); // put in argument

    if(PyCallable_Check(pFunc))
    {
        PyObject_CallObject(pFunc, pArgList);
    }
    else
    {
        PyErr_Print();
    }
}

#ifdef __cplusplus
}
#endif