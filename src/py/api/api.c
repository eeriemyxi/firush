#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdio.h>

#include "../../firush.c"

static PyObject* CompilationError;

static PyObject* fetch_html_for(PyObject* self, PyObject* args)
{
    const char* url;

    if(!PyArg_ParseTuple(args, "s", &url)) {
        return NULL;
    }

    struct ResponseStruct response;

    short err_no = fir_fetch_html_for(url, &response);

    if (err_no == 1) {
        PyErr_Format(PyExc_ValueError, "Something went wrong while fetching '%s'.", url);
        return NULL;
    }

    PyObject* pyhtml = Py_BuildValue("s", response.memory);
    
    if (!pyhtml) {
        fir_free_response(&response);
        return PyErr_NoMemory();
    }

    fir_free_response(&response);
    return pyhtml;
}

static PyObject* scrape_page_id(PyObject* self, PyObject* args)
{
    char* html;

    if(!PyArg_ParseTuple(args, "s", &html)) {
        return NULL;
    }
    
    int* ids = NULL;
    short err_no = fir_scrape_page_id(html, &ids);

    if (errno == 1) {
        PyErr_SetString(CompilationError, "Couldn't compile SCRAPE_TOTAL_PAGES.");
        return NULL;
    } else if (errno == 2) {
        return PyErr_NoMemory();
    }

    size_t ids_total = fir_ids_count(&ids);
    PyObject* py_ids = PyList_New(ids_total);
    
    if (!py_ids) {
        fir_free_ids(&ids);
        return PyErr_NoMemory();
    }

    for (size_t i = 0; i < ids_total; i++) {
        PyObject* py_id = PyLong_FromLong(ids[i]);

        if (!py_id) {
            Py_DECREF(py_ids);
            fir_free_ids(&ids);
            return PyErr_NoMemory();
        }

        PyList_SetItem(py_ids, i, py_id);
    }

    fir_free_ids(&ids);
    return py_ids;
}

static PyObject* scrape_total_pages(PyObject* self, PyObject* args)
{
    char* html;

    if(!PyArg_ParseTuple(args, "s", &html)) {
        return NULL;
    }
    
    int total_pages = 0;
    short err_no = fir_scrape_total_pages(html, &total_pages);

    if (errno == 1) {
        PyErr_SetString(CompilationError, "Couldn't compile SCRAPE_TOTAL_PAGES.");
        return NULL;
    } else if (errno == 2) {
        return PyErr_NoMemory();
    }
    
    PyObject* py_total_pages = PyLong_FromLong(total_pages);
    
    if (!py_total_pages) {
        return PyErr_NoMemory();
    }

    return py_total_pages;
}

static PyMethodDef api_methods[] = {
    { "fetch_html_for", fetch_html_for, METH_VARARGS, "Fetch HTML for the given `url` with an HTTP GET request. Raises `ValueError` if there is something wrong with the `url`." },
    { "scrape_page_id", scrape_page_id, METH_VARARGS, "Scrape page ids from the given `html`. Though very unlikely, it can raise `CompilationError` if the regular expression that it is internally using doesn't compile." },
    { "scrape_total_pages", scrape_total_pages, METH_VARARGS, "Scrape the amount of pages from the given `html`. Though very unlikely, it can raise `CompilationError` if the regular expression that it is internally using doesn't compile." },
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef api_module = {
    PyModuleDef_HEAD_INIT,
    "api",
    "Python wrapper for Firush API.",
    -1,
    api_methods
};

PyMODINIT_FUNC PyInit_api(void)
{
    PyObject *api = PyModule_Create(&api_module);
    if (!api) {
        return NULL;
    }

    CompilationError = PyErr_NewException("api.CompilationError", NULL, NULL);
    if (!CompilationError) {
        Py_DECREF(api);
        return NULL;
    }

    PyModule_AddObject(api, "CompilationError", CompilationError);

    return api;
}
