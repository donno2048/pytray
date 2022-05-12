#ifndef PYTRAY
#define PYTRAY
#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#include "Python.h"
#ifndef Py_PYTHON_H
#error "Python headers needed to compile C extensions, please install development version of Python."
#elif PY_VERSION_HEX < 0x02060000 || (0x03000000 <= PY_VERSION_HEX && PY_VERSION_HEX < 0x03030000)
#error "Cython requires Python 2.6+ or Python 3.3+."
#endif
#define _WIN32_WINNT 0x0501
#include <Windows.h>
HMENU hMenu = NULL;
HINSTANCE hInstance = NULL;
NOTIFYICONDATA nid = {sizeof(nid)};
#define __prototype(name) static PyObject * name(PyObject * self, PyObject * args)
#define add_name(func) #func, func
__prototype(remove_icon);
__prototype(create_menu);
__prototype(create_tray_element);
__prototype(add_icon);
__prototype(set_icon);
__prototype(hide_window);
__prototype(what_is_clicked);
static PyMethodDef pytray_methods[] = {
    {add_name(remove_icon), METH_NOARGS, "Remove the tray icon."},
    {add_name(create_menu), METH_VARARGS, "Create a menu."},
    {add_name(create_tray_element), METH_NOARGS, "Create a tray element."},
    {add_name(add_icon), METH_NOARGS, "Add the tray icon."},
    {add_name(set_icon), METH_VARARGS, "Set the tray icon."},
    {add_name(hide_window), METH_NOARGS, "Hide the window."},
    {add_name(what_is_clicked), METH_NOARGS, "What is clicked?"},
    {NULL, NULL, 0, NULL}
};
static struct PyModuleDef pytray_module = {PyModuleDef_HEAD_INIT, "pytray", NULL, -1, pytray_methods};
PyMODINIT_FUNC PyInit_pytray(void) {
    PyObject *m;
    m = PyModule_Create(&pytray_module);
    if (m == NULL) return NULL;
    return m;
}
__prototype(remove_icon) {
    Shell_NotifyIcon(NIM_DELETE, &nid);
    Py_RETURN_NONE;
}
__prototype(create_menu) { // FIXME: doesn't work, breaks everything
    const int elements;
    // const
    PyObject *items, *ids;
    if (!PyArg_ParseTuple(args, "iO!O!", &elements, &items, &ids)) return NULL;
    if (PyList_Size(items) != elements) return NULL;
    if (PyList_Size(ids) != elements) return NULL;
    char **items_c = malloc(elements * sizeof(char *));
    int *ids_c = malloc(elements * sizeof(int));
    for (int i = 0; i < elements; i++) {
        if (!PyUnicode_Check(PyList_GetItem(items, i))) return NULL;
        if (!PyLong_Check(PyList_GetItem(ids, i))) return NULL;
        items_c[i] = PyBytes_AS_STRING(PyUnicode_AsEncodedString(PyList_GetItem(items, i), "utf-8", "strict"));
        ids_c[i] = PyLong_AsLong(PyList_GetItem(ids, i));
    }
    hMenu = CreatePopupMenu();
    for (int i = 0; i < elements; i++) AppendMenu(hMenu, 0, ids_c[i], items_c[i]);
    Py_RETURN_NONE;
}
__prototype(create_tray_element) {
    nid.uFlags = 7;
    nid.uID = 1;
    nid.hWnd = CreateWindowEx(0, "STATIC", "", WS_POPUP, 0, 0, 0, 0, 0, hMenu, 0, 0);
    Py_RETURN_NONE;
}
void __remove_icon_c(void){Shell_NotifyIcon(NIM_DELETE, &nid);}
__prototype(add_icon) {
    hInstance = GetModuleHandle(NULL);
    Shell_NotifyIcon(NIM_ADD, &nid);
    Py_AtExit(__remove_icon_c);
    Py_RETURN_NONE;
}
__prototype(set_icon) {
    const char *icon;
    if (!PyArg_ParseTuple(args, "s", &icon)) return NULL;
    nid.hIcon = LoadImage(NULL, icon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    Shell_NotifyIcon(NIM_MODIFY, &nid);
    Py_RETURN_NONE;
}
__prototype(hide_window) {
    ShowWindow(FindWindowA("ConsoleWindowClass", NULL), SW_HIDE);
    Py_RETURN_NONE;
}
__prototype(what_is_clicked) {
    POINT p;
    if ((GetAsyncKeyState(VK_LBUTTON) | GetAsyncKeyState(VK_RBUTTON)) & 1) {
        GetCursorPos(&p);
        HWND hWnd = FindWindowEx(FindWindow("Shell_TrayWnd", NULL), NULL, "TrayNotifyWnd", NULL);
        HWND hWnd2 = FindWindowEx(hWnd, NULL, "SysPager", NULL);
        if (WindowFromPoint(p) == (hWnd2 ? FindWindowEx(hWnd2, NULL, "ToolbarWindow32", NULL) : FindWindowEx(hWnd, NULL, "ToolbarWindow32", NULL))) {
            SetForegroundWindow(nid.hWnd);
            return PyLong_FromLong(TrackPopupMenu(hMenu, TPM_RETURNCMD, p.x, p.y, 0, nid.hWnd, NULL));
        }
    }
    Py_RETURN_NONE;
}
#else
#error "This file is only for windows."
#endif
#ifdef __cplusplus
}
#endif
#endif