/*
 * pythonopcodes.c
 *
 * Copyright (C) 2002 Maurizio Umberto Puxeddu
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifdef _DEBUG
# undef _DEBUG
#  include <Python.h>
# define _DEBUG
#else
# include <Python.h>
#endif
#include <sysdep.h>
#include "csdl.h"
#include "pythonopcodes.h"
#include "pythonhelper.h"

/* HELPERS */

static CS_NOINLINE void create_private_namespace_if_needed(OPDS *o)
{
  /* PyLocal variable is now a global variable indexed by INSDS */
  char insds[32];
  CSOUND *csound = o->insdshead->csound;
  snprintf(insds, 32, "PYLOCAL:%p", o->insdshead);
  if(csound->QueryGlobalVariable(csound,insds) == NULL) {
    csound->CreateGlobalVariable(csound,insds, sizeof(PyObject *));
    SETPYLOCAL(o->insdshead, PyDict_New());
#ifdef DEBUG_PY_NAMESPACES
      printf("Creating private namespace %p for %p\n",
             (void*) GETPYLOCAL(o->insdshead), (void*) o->insdshead);
#endif
    }
#ifdef DEBUG_PY_NAMESPACES
    else {
      printf("Private namespace for %p already allocated at %p\n",
             (void*) o->insdshead, (void*) GETPYLOCAL(o->insdshead));
    }
#endif
}

static void format_call_statement2(char *statement, char *callable,
                                  int argc, MYFLT *argv[], long skip)
{
    int       i;

    statement[0] = '\0';
    if (argc-skip > 0) {
      snprintf(statement, 1024, "%s(%0.6f", callable, *(argv[skip]));
      for (i = skip+1; i < argc; ++i) {
        snprintf(statement + strlen(statement), 1024 - strlen(statement),
                ", %f", *(argv[i]));
      }
      // MKG 2014 Jan 29: No linkage for strlcat in py.dll on MinGW.
      //strlcat(statement, ")", 1024);
      strncat(statement, ")", 1023 - strlen(statement)); statement[1023] = '\0';
    }
    else {
      snprintf(statement, 1024, "%s()", callable);
    }

}

static void format_call_statement(char *statement, char *callable,
                                  int argc, MYFLT *argv[], int skip)
{
    int       i;

    statement[0] = '\0';
    if (argc > 0) {
      snprintf(statement, 1024, "%s(%0.6f", callable, *(argv[0]));
      for (i = 1; i < argc - skip; ++i) {
        snprintf(statement + strlen(statement), 1024-strlen(statement),
                 ", %f", *(argv[i]));
      }
      // MKG 2014 Jan 29: No linkage for strlcat in py.dll on MinGW.
      //strlcat(statement, ")", 1024);
      strncat(statement, ")", 1023-strlen(statement)); statement[1023] = '\0';
    }
    else {
      snprintf(statement, 1024, "%s()", callable);
    }
}


static PyObject *
run_statement_in_given_context(char *string, PyObject *private)
{
    PyObject  *module, *public;

    module = PyImport_AddModule("__main__");
    if (module == NULL) {
      PyErr_SetString(PyExc_RuntimeError, "couldn't find module __main__");
      return NULL;
    }
    public = PyModule_GetDict(module);
    return PyRun_String(string, Py_file_input,
                        public, private ? private : public);
}

static PyObject *
eval_string_in_given_context(char *string, PyObject *private)
{
    PyObject  *module, *public;

    module = PyImport_AddModule("__main__");
    if (module == NULL) {
      PyErr_SetString(PyExc_RuntimeError, "couldn't find module __main__");
      return NULL;
    }
    public = PyModule_GetDict(module);
    return PyRun_String(string, Py_eval_input,
                        public, private ? private : public);
}

static PyObject *
exec_file_in_given_context(CSOUND* cs, char *filename, PyObject *private)
{
    PyObject  *result, *module, *public;
    char      *fullpath;

    fullpath = cs->FindInputFile(cs, filename, NULL);

    module = PyImport_AddModule("__main__");
    if (module == NULL) {
      PyErr_SetString(PyExc_RuntimeError, "couldn't find module __main__");
      return NULL;
    }
    public = PyModule_GetDict(module);
    FILE *fd = fopen(fullpath, "r");
    if (!fd) {
      PyErr_Format(PyExc_RuntimeError,
                   "couldn't open script file %s", filename);
      return NULL;
    }
    result = PyRun_File(fd, fullpath, Py_file_input,
                    public, private ? private : public);
    fclose(fd);
    return result;
}

/* ------ OPCODES ------ */

static CS_NOINLINE int errMsg(void *p, const char *msg)
{
    CSOUND      *csound = ((OPDS*) p)->insdshead->csound;
    const char  *opname = GetOpcodeName(p);
    csound->ErrorMsg(csound, "%s: %s", opname, msg);

    return NOTOK;
}

static CS_NOINLINE int pyErrMsg(void *p, const char *msg)
{
    CSOUND      *csound = ((OPDS*) p)->insdshead->csound;
    const char  *opname = GetOpcodeName(p);
    csound->ErrorMsg(csound, "%s: %s", opname, msg);
    PyErr_Print();

    return NOTOK;
}

static int pyinit(CSOUND *csound, PYINIT *p)
{
    (void) csound;
    (void) p;
    int *py_initialize_done;

    if((py_initialize_done = csound->QueryGlobalVariable(csound,
                                                         "PY_INITIALIZE")) == NULL){
    csound->CreateGlobalVariable(csound, "PY_INITIALIZE", sizeof(int));
    py_initialize_done = csound->QueryGlobalVariable(csound,"PY_INITIALIZE");
    *py_initialize_done = 0;
    }

    if (*py_initialize_done == 0) {
      Py_Initialize();
      *py_initialize_done = 1;
    }

    PyObject *module = PyImport_AddModule("__main__");
    if (module == NULL) {
      PyErr_SetString(PyExc_RuntimeError, "couldn't find module __main__");
      return NOTOK;
    }
    PyObject *public = PyModule_GetDict(module);
    PyObject *csobj = Py_BuildValue("l", (uintptr_t) csound);
    PyDict_SetItemString(public, "_CSOUND_", csobj);
    return OK;
}

#include "pyx.auto.c"
#include "pycall.auto.c"

static int pycalln_krate(CSOUND *csound, PYCALLN *p)
{
    IGN(csound);
    int       i;
    char      command[1024];
    PyObject  *result;


    format_call_statement2(command,  p->function->data,
                          p->INOCOUNT-2, p->args, *p->nresult);
    result = eval_string_in_given_context(command, 0);
    if (result != NULL && PyTuple_Check(result) &&
        PyTuple_Size(result) == (int) *p->nresult) {
      for (i = 0; i < *p->nresult; ++i)
        *p->args[i] = PyFloat_AsDouble(PyTuple_GET_ITEM(result, i));
      Py_DECREF(result);
    }
    else {
      return pyErrMsg(p, "ERROR");
    }
    return OK;
}

static int pylcalln_irate(CSOUND *csound, PYCALLN *p)
{
     IGN(csound);
    create_private_namespace_if_needed(&p->h);
    return OK;
}

static int pylcalln_krate(CSOUND *csound, PYCALLN *p)
{
     IGN(csound);
    int       i;
    char      command[1024];
    PyObject  *result;

    format_call_statement2(command,  p->function->data,
                          p->INOCOUNT-2, p->args, *p->nresult);
    result = eval_string_in_given_context(command, GETPYLOCAL(p->h.insdshead));
    if (result != NULL && PyTuple_Check(result) &&
        PyTuple_Size(result) == (int) *p->nresult) {
      for (i = 0; i < *p->nresult; ++i)
        *p->args[i] = PyFloat_AsDouble(PyTuple_GET_ITEM(result, i));
      Py_DECREF(result);
    }
    else {
      return pyErrMsg(p, "ERROR");
    }
    return OK;
}

static int pylcallni_irate(CSOUND *csound, PYCALLN *p)
{
     IGN(csound);
    int       i;
    char      command[1024];
    PyObject  *result;

    create_private_namespace_if_needed(&p->h);
    format_call_statement2(command,  p->function->data,
                          p->INOCOUNT-2, p->args, *p->nresult);
    result = eval_string_in_given_context(command, GETPYLOCAL(p->h.insdshead));
    if (result != NULL && PyTuple_Check(result) &&
        PyTuple_Size(result) == (int) *p->nresult) {
      for (i = 0; i < *p->nresult; ++i)
        *p->args[i] = PyFloat_AsDouble(PyTuple_GET_ITEM(result, i));
      Py_DECREF(result);
    }
    else {
      return pyErrMsg(p, "ERROR");
    }
    return OK;
}

  /* PYTHON OPCODES */

OENTRY python_localops[] = {

  /* INITIALIZATION */

{ "pyinit",   sizeof(PYINIT),   0,   "",     "",     (SUBR)pyinit, NULL      },

  /* RUN GROUP */
{ "pyrun",    sizeof(PYRUN),    0,   "",     "S",    NULL, (SUBR)pyrun_krate },
{ "pyruni",   sizeof(PYRUN),    0,   "",     "S",    (SUBR)pyruni_irate      },
{ "pylrun",   sizeof(PYRUN),    0,   "",     "S",
              (SUBR)pylrun_irate, (SUBR)pylrun_krate },
{ "pylruni",  sizeof(PYRUN),    0,   "",     "S",    (SUBR)pylruni_irate     },

{ "pyrunt",   sizeof(PYRUNT),   0,   "",     "kS",   NULL, (SUBR)pyrunt_krate },
{ "pylrunt",  sizeof(PYRUNT),   0,   "",     "kS",
              (SUBR)pylrunt_irate, (SUBR)pylrunt_krate },

  /* EXEC GROUP */

{ "pyexec",   sizeof(PYEXEC),   0,   "",     "S",    NULL, (SUBR)pyexec_krate },
{ "pyexeci",  sizeof(PYEXEC),   0,   "",     "S",    (SUBR)pyexec_krate      },
{ "pylexec",  sizeof(PYEXEC),   0,   "",     "S",
              (SUBR)pylexec_irate, (SUBR)pylexec_krate },
{ "pylexeci", sizeof(PYEXEC),   0,   "",     "S",    (SUBR)pylexeci_irate    },

{ "pyexect",  sizeof(PYEXECT),  0,   "",     "kS",   NULL, (SUBR)pyexect_krate },
{ "pylexect", sizeof(PYEXECT),  0,   "",     "kS",
              (SUBR)pylexect_irate, (SUBR)pylexect_krate },

  /* CALL GROUP */

{ "pycall",   sizeof(PYCALL0),  0,   "" ,    "Sz",   NULL, (SUBR)pycall0_krate },
{ "pycall1",  sizeof(PYCALL1),  0,   "k",    "Sz",   NULL, (SUBR)pycall1_krate },
{ "pycall2",  sizeof(PYCALL2),  0,   "kk",   "Sz",   NULL, (SUBR)pycall2_krate },
{ "pycall3",  sizeof(PYCALL3),  0,   "kkk",  "Sz",   NULL, (SUBR)pycall3_krate },
{ "pycall4",  sizeof(PYCALL4),  0,   "kkkk", "Sz",   NULL, (SUBR)pycall4_krate },
{ "pycall5",  sizeof(PYCALL5),  0,   "kkkkk", "Sz",  NULL, (SUBR)pycall5_krate },
{ "pycall6",  sizeof(PYCALL6),  0,   "kkkkkk", "Sz", NULL, (SUBR)pycall6_krate },
{ "pycall7",  sizeof(PYCALL7),  0,   "kkkkkkk", "Sz", NULL, (SUBR)pycall7_krate },
{ "pycall8",  sizeof(PYCALL8), 0,  "kkkkkkkk", "Sz", NULL,(SUBR)pycall8_krate },

{ "pycalln",  sizeof(PYCALLN),  0,   "",     "Siz",  NULL, (SUBR)pycalln_krate },

{ "pycallt",  sizeof(PYCALL0T), 0,   "" ,    "kSz",  NULL, (SUBR)pycall0t_krate },
{ "pycall1t", sizeof(PYCALL1T), 0,   "k",    "kSz",  NULL, (SUBR)pycall1t_krate },
{ "pycall2t", sizeof(PYCALL2T), 0,   "kk",   "kSz",  NULL, (SUBR)pycall2t_krate },
{ "pycall3t", sizeof(PYCALL3T),0,    "kkk",  "kSz",  NULL, (SUBR)pycall3t_krate },
{ "pycall4t", sizeof(PYCALL4T), 0,   "kkkk", "kSz",  NULL, (SUBR)pycall4t_krate },
{ "pycall5t", sizeof(PYCALL5T), 0,   "kkkkk", "kSz", NULL, (SUBR)pycall5t_krate },
{ "pycall6t", sizeof(PYCALL6T), 0,   "kkkkkk", "kSz", NULL,(SUBR)pycall6t_krate },
{ "pycall7t", sizeof(PYCALL7T), 0,   "kkkkkkk","kSz", NULL,(SUBR)pycall7t_krate },
{ "pycall8t", sizeof(PYCALL8T), 0,  "kkkkkkkk","kSz",NULL,(SUBR)pycall8t_krate },

#if 0
{ "pycallnt", sizeof(PYCALLNT), 0,   "",  "Siz",  NULL, (SUBR)pycallnt_krate },
#endif

{ "pycalli",  sizeof(PYCALL0),  0,   "",         "Sm",   (SUBR)pycall0_krate },
{ "pycall1i", sizeof(PYCALL1),  0,   "i",        "Sm",   (SUBR)pycall1_krate },
{ "pycall2i", sizeof(PYCALL2),  0,   "ii",       "Sm",   (SUBR)pycall2_krate },
{ "pycall3i", sizeof(PYCALL3),  0,   "iii",      "Sm",   (SUBR)pycall3_krate },
{ "pycall4i", sizeof(PYCALL4),  0,   "iiii",     "Sm",   (SUBR)pycall4_krate },
{ "pycall5i", sizeof(PYCALL5),  0,   "iiiii",    "Sm",   (SUBR)pycall5_krate },
{ "pycall6i", sizeof(PYCALL6),  0,   "iiiiii",   "Sm",   (SUBR)pycall6_krate },
{ "pycall7i", sizeof(PYCALL7),  0,   "iiiiiii",  "Sm",   (SUBR)pycall7_krate },
{ "pycall8i", sizeof(PYCALL8),  0,   "iiiiiiii", "Sm",   (SUBR)pycall8_krate },

{ "pycallni", sizeof(PYCALLN),  0,   "",         "Sim",  (SUBR)pycalln_krate },

{ "pylcall",  sizeof(PYCALL0),  0,   "" ,        "Sz",
              (SUBR)pylcall0_irate, (SUBR)pylcall0_krate },
{ "pylcall1", sizeof(PYCALL1),  0,   "k",        "Sz",
              (SUBR)pylcall1_irate, (SUBR)pylcall1_krate },
{ "pylcall2", sizeof(PYCALL2),  0,   "kk",       "Sz",
              (SUBR)pylcall2_irate, (SUBR)pylcall2_krate },
{ "pylcall3", sizeof(PYCALL3),  0,   "kkk",      "Sz",
              (SUBR)pylcall3_irate, (SUBR)pylcall3_krate },
{ "pylcall4", sizeof(PYCALL4),  0,   "kkkk",     "Sz",
              (SUBR)pylcall4_irate, (SUBR)pylcall4_krate },
{ "pylcall5", sizeof(PYCALL5),  0,   "kkkkk",    "Sz",
              (SUBR)pylcall5_irate, (SUBR)pylcall5_krate },
{ "pylcall6", sizeof(PYCALL6),  0,   "kkkkkk",   "Sz",
              (SUBR)pylcall6_irate, (SUBR)pylcall6_krate },
{ "pylcall7", sizeof(PYCALL7),  0,   "kkkkkkk",  "Sz",
              (SUBR)pylcall7_irate, (SUBR)pylcall7_krate },
{ "pylcall8", sizeof(PYCALL8),  0,   "kkkkkkkk", "Sz",
              (SUBR)pylcall8_irate, (SUBR)pylcall8_krate },

{ "pylcalln", sizeof(PYCALLN),  0,   "",         "Siz",
              (SUBR)pylcalln_irate, (SUBR)pylcalln_krate },

{ "pylcallt", sizeof(PYCALL0T), 0,   "" ,        "kSz",
              (SUBR)pylcall0t_irate, (SUBR)pylcall0t_krate },
{ "pylcall1t", sizeof(PYCALL1T), 0,  "k",        "kSz",
               (SUBR)pylcall1t_irate, (SUBR)pylcall1t_krate },
{ "pylcall2t", sizeof(PYCALL2T), 0,  "kk",       "kSz",
               (SUBR)pylcall2t_irate, (SUBR)pylcall2t_krate },
{ "pylcall3t", sizeof(PYCALL3T), 0,  "kkk",      "kSz",
               (SUBR)pylcall3t_irate, (SUBR)pylcall3t_krate },
{ "pylcall4t", sizeof(PYCALL4T), 0,  "kkkk",     "kSz",
               (SUBR)pylcall4t_irate, (SUBR)pylcall4t_krate },
{ "pylcall5t", sizeof(PYCALL5T), 0,  "kkkkk",    "kSz",
               (SUBR)pylcall5t_irate, (SUBR)pylcall5t_krate },
{ "pylcall6t", sizeof(PYCALL6T), 0,  "kkkkkk",   "kSz",
               (SUBR)pylcall6t_irate, (SUBR)pylcall6t_krate },
{ "pylcall7t", sizeof(PYCALL7T), 0,  "kkkkkkk",  "kSz",
               (SUBR)pylcall7t_irate, (SUBR)pylcall7t_krate },
{ "pylcall8t", sizeof(PYCALL8T), 0,  "kkkkkkkk", "kSz",
               (SUBR)pylcall8t_irate, (SUBR)pylcall8t_krate },

#if 0
{ "pylcallnt", sizeof(PYCALLNT), 0,  "",         "Siz",
               (SUBR)pylcalln_irate, (SUBR)pylcallnt_krate },
#endif

{ "pylcalli", sizeof(PYCALL0),  0,  "",         "Sm",   (SUBR)pylcall0i_irate },
{ "pylcall1i", sizeof(PYCALL1), 0,  "i",        "Sm",   (SUBR)pylcall1i_irate },
{ "pylcall2i", sizeof(PYCALL2), 0,  "ii",       "Sm",   (SUBR)pylcall2i_irate },
{ "pylcall3i", sizeof(PYCALL3), 0,  "iii",      "Sm",   (SUBR)pylcall3i_irate },
{ "pylcall4i", sizeof(PYCALL4), 0,  "iiii",     "Sm",   (SUBR)pylcall4i_irate },
{ "pylcall5i", sizeof(PYCALL5), 0,  "iiiii",    "Sm",   (SUBR)pylcall5i_irate },
{ "pylcall6i", sizeof(PYCALL6), 0,  "iiiiii",   "Sm",   (SUBR)pylcall6i_irate },
{ "pylcall7i", sizeof(PYCALL7), 0,   "iiiiiii",  "Sm",   (SUBR)pylcall7i_irate },
{ "pylcall8i", sizeof(PYCALL8), 0,  "iiiiiiii", "Sm",   (SUBR)pylcall8i_irate },

{ "pylcallni", sizeof(PYCALLN), 0,   "",         "Sim",  (SUBR)pylcallni_irate },

  /* EVAL GROUP */

{ "pyeval",   sizeof(PYEVAL),   0,  "k",    "S",    NULL, (SUBR)pyeval_krate },
{ "pyevali",  sizeof(PYEVAL),   0,  "i",    "S",    (SUBR)pyeval_krate },
{ "pyleval",  sizeof(PYEVAL),   0,   "k",    "S",
              (SUBR)pyleval_irate, (SUBR)pyleval_krate },
{ "pylevali", sizeof(PYEVAL),   0,   "i",    "S",    (SUBR)pylevali_irate },

{ "pyevalt",  sizeof(PYEVALT),  0,   "k",    "S",    NULL, (SUBR)pyevalt_krate },
{ "pylevalt", sizeof(PYEVALT),  0,   "k",    "S",
              (SUBR)pylevalt_irate, (SUBR)pylevalt_krate },

  /* ASSIGN GROUP */

{ "pyassign", sizeof(PYASSIGN), 0,   "",     "Sz",   NULL, (SUBR)pyassign_krate },
{ "pyassigni", sizeof(PYASSIGN), 0,  "",     "Sz",   (SUBR)pyassign_krate },
{ "pylassign", sizeof(PYASSIGN), 0,  "",     "Sz",
               (SUBR)pylassign_irate, (SUBR)pylassign_krate },
{ "pylassigni", sizeof(PYASSIGN), 0,  "",    "Sz",   (SUBR)pylassigni_irate },

{ "pyassignt", sizeof(PYASSIGNT), 0,  "",    "Sz", NULL, (SUBR)pyassignt_krate },
{ "pylassignt", sizeof(PYASSIGNT), 0, "",   "Sz",
                (SUBR)pylassignt_irate, (SUBR)pylassignt_krate },

};

LINKAGE_BUILTIN(python_localops)
