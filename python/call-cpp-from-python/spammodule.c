#define PY_SSIZE_T_CLEAN

#include <Python.h>

static PyObject* spam_system(PyObject *self, PyObject *args)
{
	const char* cmd;
	int sts;
	if(!PyArg_ParseTuple(args, "s", &cmd)) return NULL;

	sts = system(cmd);
	return PyLong_FromLong(sts);


}

static PyMethodDef SpamMethods[] = {
  {"system", spam_system, METH_VARARGS, "Execute a shell command" },
  {NULL, NULL, 0, NULL }
};

static struct PyModuleDef spammodule = {
  PyModuleDef_HEAD_INIT,
  "spam",
  NULL,
  -1,
  SpamMethods
};


PyMODINIT_FUNC
PyInit_spam(void)
{
	  return PyModule_Create(&spammodule);
}

