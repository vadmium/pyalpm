/**
 * db.c : wrapper class around pmdb_t
 *
 *  Copyright (c) 2011 Rémy Oudompheng <remy@archlinux.org>
 *
 *  This file is part of pyalpm.
 *
 *  pyalpm is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  pyalpm is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with pyalpm.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <alpm.h>
#include <Python.h>
#include "db.h"
#include "package.h"
#include "util.h"

typedef struct _AlpmDB {
  PyObject_HEAD
  pmdb_t *c_data;
} AlpmDB;

static void pyalpm_db_dealloc(AlpmDB *self) {
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* pyalpm_db_get_pkg(PyObject* self, PyObject* args) {
  char *pkgname;
  pmpkg_t *p;
  AlpmDB *db = (AlpmDB *)self;

  if(!PyArg_ParseTuple(args, "s", &pkgname))
  {
    PyErr_SetString(PyExc_TypeError, "get_pkg() takes a string argument");
    return NULL;
  }

  p = alpm_db_get_pkg(db->c_data, pkgname);

  if (p == NULL) {
    Py_RETURN_NONE;
  } else {
    PyObject *result;
    result = pyalpm_package_from_pmpkg(p);
    if (result == NULL) {
      return NULL;
    } else {
      return result;
    }
  }
}

static PyObject* pyalpm_db_get_pkgcache(PyObject* self, PyObject* _dummy) {
  AlpmDB *db = (AlpmDB *)self;
  alpm_list_t *pkglist;

  pkglist = alpm_db_get_pkgcache(db->c_data);
  return alpmlist_to_pylist(pkglist, pyalpm_package_from_pmpkg);
}

static struct PyMethodDef db_methods[] = {
  { "get_pkg", pyalpm_db_get_pkg, METH_VARARGS, "get a package by name" },
  { "get_pkgcache", pyalpm_db_get_pkgcache, METH_NOARGS, "get the DB package list" },
  { NULL },
};

PyTypeObject AlpmDBType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "alpm.DB",             /*tp_name*/
  sizeof(AlpmDB),        /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  (destructor)pyalpm_db_dealloc,     /*tp_dealloc*/
  0,                          /*tp_print*/
  0,                          /*tp_getattr*/
  0,                          /*tp_setattr*/
  NULL,                       /*tp_reserved*/
  0,                          /*tp_repr*/
  0,                          /*tp_as_number*/
  0,                          /*tp_as_sequence*/
  0,                          /*tp_as_mapping*/
  0,                          /*tp_hash */
  0,                          /*tp_call*/
  0,                          /*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "libalpm DB object",        /* tp_doc */
  0,                          /* tp_traverse */
  0,                          /* tp_clear */
  0,                          /* tp_richcompare */
  0,                          /* tp_weaklistoffset */
  0,                          /* tp_iter */
  0,                          /* tp_iternext */
  db_methods,                          /* tp_methods */
  0,                          /* tp_members */
  0,                          /* tp_getset */
  0,                          /* tp_base */
  0,                          /* tp_dict */
  0,                          /* tp_descr_get */
  0,                          /* tp_descr_set */
  0,                          /* tp_dictoffset */
  0,                          /* tp_init */
  0,                          /* tp_alloc */
  0,                          /* tp_new */
};

/** Initializes Pb class in module */
void init_pyalpm_db(PyObject *module) {
  PyObject *type;

  if (PyType_Ready(&AlpmDBType) < 0)
    return;
  type = (PyObject*)&AlpmDBType;
  Py_INCREF(type);
  PyModule_AddObject(module, "DB", type);
}

PyObject *pyalpm_db_from_pmdb(pmdb_t *db) {
  AlpmDB *self;
  self = (AlpmDB*)AlpmDBType.tp_alloc(&AlpmDBType, 0);
  if (self == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to create DB object");
    return NULL;
  }

  self->c_data = db;
  return (PyObject *)self;
}
