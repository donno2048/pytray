#ifndef PYTRAY
#define PYTRAY
#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#include "Python.h"
#define _WIN32_WINNT 0x0501
#include <Windows.h>
#define __prototype(name) static PyObject * name(PyObject * self, PyObject * args)
#define add_name(func) #func, func
#define T_Error(msg) {PyErr_SetString(PyExc_TypeError, msg); return NULL;}
HMENU hMenu = NULL;
HINSTANCE hInstance = NULL;
NOTIFYICONDATA nid = {sizeof(nid)};
void __remove_icon_c(void){Shell_NotifyIcon(NIM_DELETE, &nid);}
PyObject * create_menu(PyObject *self, PyObject *const *args, Py_ssize_t nargs);
__prototype(remove_icon);
__prototype(create_tray_element);
__prototype(add_icon);
__prototype(set_icon);
__prototype(hide_window);
__prototype(hide_menu);
__prototype(what_is_clicked);
static PyMethodDef pytray_methods[] = {
    {add_name(remove_icon), METH_NOARGS, "Remove the tray icon."},
    {add_name(create_menu), METH_FASTCALL, "Create a menu."},
    {add_name(create_tray_element), METH_NOARGS, "Create a tray element."},
    {add_name(add_icon), METH_NOARGS, "Add the tray icon."},
    {add_name(set_icon), METH_O, "Set the tray icon."},
    {add_name(hide_window), METH_NOARGS, "Hide the window."},
    {add_name(hide_menu), METH_NOARGS, "Hide the menu."},
    {add_name(what_is_clicked), METH_NOARGS, "What is clicked?"},
    {NULL, NULL, 0, NULL}
};
static struct PyModuleDef pytray_module = {PyModuleDef_HEAD_INIT, "pytray", NULL, -1, pytray_methods};
PyMODINIT_FUNC PyInit_pytray(void){return PyModule_Create(&pytray_module);}
PyObject * create_menu(PyObject *self, PyObject *const *args, Py_ssize_t elements) {
    hMenu = CreatePopupMenu();
    for (int i = 0; i < elements; i++) {
        if (!PyTuple_Check(args[i])) T_Error("Argument must be a tuple.");
        if (PyTuple_Size(args[i]) != 2) T_Error("Tuple must have 2 elements.");
        if (!PyUnicode_Check(PyTuple_GetItem(args[i], 0))) T_Error("First element must be a string.");
        if (!PyLong_Check(PyTuple_GetItem(args[i], 1))) T_Error("Second element must be an integer.");
        AppendMenu(hMenu, MF_STRING, PyLong_AsLong(PyTuple_GetItem(args[i], 1)), PyUnicode_AsUTF8(PyTuple_GetItem(args[i], 0)));
    }
    Py_RETURN_NONE;
}
__prototype(remove_icon) {
    Shell_NotifyIcon(NIM_DELETE, &nid);
    Py_RETURN_NONE;
}
__prototype(create_tray_element) {
    nid.uFlags = 7;
    nid.uID = 1;
    nid.hWnd = CreateWindowEx(0, "STATIC", "", WS_POPUP, 0, 0, 0, 0, 0, hMenu, 0, 0);
    Py_RETURN_NONE;
}
__prototype(add_icon) {
    hInstance = GetModuleHandle(NULL);
    Shell_NotifyIcon(NIM_ADD, &nid);
    Py_AtExit(__remove_icon_c);
    Py_RETURN_NONE;
}
__prototype(set_icon) {
    const char *icon;
    if (!PyUnicode_Check(args)) T_Error("Argument must be a string.");
    icon = PyUnicode_AsUTF8(args);
    if (GetFileAttributes(icon) == INVALID_FILE_ATTRIBUTES) {
        PyErr_SetString(PyExc_FileNotFoundError, "File not found.");
        return NULL;
    }
    if (GetFileAttributes(icon) & FILE_ATTRIBUTE_DIRECTORY) T_Error("File is a directory.");
    nid.hIcon = LoadImage(NULL, icon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    Shell_NotifyIcon(NIM_MODIFY, &nid);
    Py_RETURN_NONE;
}
__prototype(hide_window) {
    ShowWindow(FindWindowA("ConsoleWindowClass", NULL), SW_HIDE);
    Py_RETURN_NONE;
}
__prototype(hide_menu) {
    RemoveMenu(hMenu, 0, 0);
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
            return PyLong_FromLong(TrackPopupMenu(hMenu, TPM_RETURNCMD, p.x - 32, p.y - 32, 0, nid.hWnd, NULL)); // random offset
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
