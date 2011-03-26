/**
 * package.c : wrapper class around pmpkg_t
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
#include "package.h"
#include "util.h"

extern PyObject* alpm_error;

typedef struct _AlpmPackage {
  PyObject_HEAD
  pmpkg_t *c_data;
} AlpmPackage;

static void pyalpm_package_dealloc(AlpmPackage *self) {
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* pyobject_from_pmdepend(void* dep) {
  char *depstring = alpm_dep_compute_string((pmdepend_t*)dep);
  PyObject *item = Py_BuildValue("s", depstring);
  free(depstring);
  return item;
};

struct PyGetSetDef AlpmPackageGetSet[];

PyTypeObject AlpmPackageType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "alpm.Package",             /*tp_name*/
  sizeof(AlpmPackage),        /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  (destructor)pyalpm_package_dealloc,     /*tp_dealloc*/
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
  "Package object",            /* tp_doc */
  0,                          /* tp_traverse */
  0,                          /* tp_clear */
  0,                          /* tp_richcompare */
  0,                          /* tp_weaklistoffset */
  0,                          /* tp_iter */
  0,                          /* tp_iternext */
  0,                          /* tp_methods */
  0,                          /* tp_members */
  AlpmPackageGetSet,          /* tp_getset */
};

/** Initializes Package class in module */
void init_pyalpm_package(PyObject *module) {
  PyObject *type;

  if (PyType_Ready(&AlpmPackageType) < 0)
    return;
  type = (PyObject*)&AlpmPackageType;
  Py_INCREF(type);
  PyModule_AddObject(module, "Package", type);
}

PyObject *pyalpm_package_from_pmpkg(pmpkg_t *p) {
  AlpmPackage *self;
  self = (AlpmPackage*)AlpmPackageType.tp_alloc(&AlpmPackageType, 0);
  if (self == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to create package object");
    return NULL;
  }

  self->c_data = p;
  return (PyObject *)self;
}

#define CHECK_IF_INITIALIZED() if (! self->c_data) { \
  PyErr_SetString(alpm_error, "data is not initialized"); \
  return NULL; \
  }

static PyObject *pyalpm_package_get_filename(AlpmPackage *self, void *closure) {
  const char *filename;

  CHECK_IF_INITIALIZED();

  filename = alpm_pkg_get_filename(self->c_data);
  if (filename == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get filename");
    return NULL;
  }
  return Py_BuildValue("s", filename);
}

static PyObject* pyalpm_package_get_name(AlpmPackage *self, void *closure) {
  const char *name;

  CHECK_IF_INITIALIZED();

  name = alpm_pkg_get_name(self->c_data);
  if (name == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get name");
    return NULL;
  }
  return Py_BuildValue("s", name);
}

static PyObject* pyalpm_package_get_version(AlpmPackage *self, void *closure) {
  const char *version;

  CHECK_IF_INITIALIZED();

  version = alpm_pkg_get_version(self->c_data);
  if (version == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get version");
    return NULL;
  }
  return Py_BuildValue("s", version);
}

static PyObject* pyalpm_package_get_desc(AlpmPackage *self, void *closure) {
  const char *desc;

  CHECK_IF_INITIALIZED();

  desc = alpm_pkg_get_desc(self->c_data);
  if (desc == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get desc");
    return NULL;
  }
  return Py_BuildValue("s", desc);
}

static PyObject* pyalpm_package_get_url(AlpmPackage *self, void *closure) {
  const char *url;

  CHECK_IF_INITIALIZED();

  url = alpm_pkg_get_url(self->c_data);
  if (url == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get url");
    return NULL;
  }
  return Py_BuildValue("s", url);
}

static PyObject* pyalpm_package_get_packager(AlpmPackage *self, void *closure) {
  const char *packager;

  CHECK_IF_INITIALIZED();

  packager = alpm_pkg_get_packager(self->c_data);
  if (packager == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get packager");
    return NULL;
  }
  return Py_BuildValue("s", packager);
}

static PyObject* pyalpm_package_get_md5sum(AlpmPackage *self, void *closure) {
  const char *md5sum;

  CHECK_IF_INITIALIZED();

  md5sum = alpm_pkg_get_md5sum(self->c_data);
  if (md5sum == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get md5sum");
    return NULL;
  }
  return Py_BuildValue("s", md5sum);
}

static PyObject* pyalpm_package_get_arch(AlpmPackage *self, void *closure) {
  const char *arch;

  CHECK_IF_INITIALIZED();

  arch = alpm_pkg_get_arch(self->c_data);
  if (arch == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get arch");
    return NULL;
  }
  return Py_BuildValue("s", arch);
}

static PyObject* pyalpm_package_get_licenses(AlpmPackage *self, void *closure) {
  alpm_list_t *licenses = NULL;

  CHECK_IF_INITIALIZED();

  licenses = alpm_pkg_get_licenses(self->c_data);
  return alpmlist_to_pylist(licenses, pyobject_from_string);
}

static PyObject* pyalpm_package_get_groups(AlpmPackage *self, void *closure) {
  alpm_list_t *groups = NULL;

  CHECK_IF_INITIALIZED();

  groups = alpm_pkg_get_groups(self->c_data);
  return alpmlist_to_pylist(groups, pyobject_from_string);
}

static PyObject* pyalpm_package_get_depends(AlpmPackage *self, void *closure) {
  alpm_list_t *depends = NULL;

  CHECK_IF_INITIALIZED();

  depends = alpm_pkg_get_depends(self->c_data);
  return alpmlist_to_pylist(depends, pyobject_from_pmdepend);
}

static PyObject* pyalpm_package_get_optdepends(AlpmPackage *self, void *closure) {
  alpm_list_t *optdepends = NULL;

  CHECK_IF_INITIALIZED();

  optdepends = alpm_pkg_get_optdepends(self->c_data);
  return alpmlist_to_pylist(optdepends, pyobject_from_string);
}

static PyObject* pyalpm_package_get_conflicts(AlpmPackage *self, void *closure) {
  alpm_list_t *conflicts = NULL;

  CHECK_IF_INITIALIZED();

  conflicts = alpm_pkg_get_conflicts(self->c_data);
  return alpmlist_to_pylist(conflicts, pyobject_from_string);
}

static PyObject* pyalpm_package_get_provides(AlpmPackage *self, void *closure) {
  alpm_list_t *provides = NULL;

  CHECK_IF_INITIALIZED();

  provides = alpm_pkg_get_provides(self->c_data);
  return alpmlist_to_pylist(provides, pyobject_from_string);
}

static PyObject* pyalpm_package_get_replaces(AlpmPackage *self, void *closure) {
  alpm_list_t *replaces = NULL;

  CHECK_IF_INITIALIZED();

  replaces = alpm_pkg_get_replaces(self->c_data);
  return alpmlist_to_pylist(replaces, pyobject_from_string);
}

static PyObject* pyalpm_package_get_files(AlpmPackage *self, void *closure) {
  alpm_list_t *files = NULL;

  CHECK_IF_INITIALIZED();

  files = alpm_pkg_get_files(self->c_data);
  return alpmlist_to_pylist(files, pyobject_from_string);
}

struct PyGetSetDef AlpmPackageGetSet[] = {
  { "name", (getter)pyalpm_package_get_name, 0, "package name", NULL } ,
  { "version", (getter)pyalpm_package_get_version, 0, "package version", NULL } ,
  { "desc", (getter)pyalpm_package_get_desc, 0, "package desc", NULL } ,
  { "url", (getter)pyalpm_package_get_url, 0, "package URL", NULL } ,
  { "packager", (getter)pyalpm_package_get_packager, 0, "packager name", NULL } ,
  { "md5sum", (getter)pyalpm_package_get_md5sum, 0, "package md5sum", NULL } ,
  { "arch", (getter)pyalpm_package_get_arch, 0, "target architecture", NULL } ,
  { "filename", (getter)pyalpm_package_get_filename, 0, "package filename", NULL } ,
  { "licenses", (getter)pyalpm_package_get_licenses, 0, "list of licenses", NULL } ,
  { "groups", (getter)pyalpm_package_get_groups, 0, "list of package groups", NULL } ,
  { "depends", (getter)pyalpm_package_get_depends, 0, "list of dependencies", NULL } ,
  { "optdepends", (getter)pyalpm_package_get_optdepends, 0, "list of optional dependencies", NULL } ,
  { "conflicts", (getter)pyalpm_package_get_conflicts, 0, "list of conflicts", NULL } ,
  { "provides", (getter)pyalpm_package_get_provides, 0, "list of provided package names", NULL } ,
  { "replaces", (getter)pyalpm_package_get_replaces, 0, "list of replaced packages", NULL } ,
  { "files", (getter)pyalpm_package_get_files, 0, "list of installed files", NULL } ,
  { NULL }
};
