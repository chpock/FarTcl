
#define POINTER_STR_LENGTH 50

/* Convert Tcl Obj to C, Free converted values */

#define AssignParam_TclToC_GUID(OBJNUM, PNAME) \
  UuidFromStringA(reinterpret_cast<unsigned char *>(Tcl_GetStringFromObj(objv[OBJNUM], NULL)), &PNAME);
#define InitParam_TclToC_GUID(OBJNUM, PNAME) \
  GUID PNAME; \
  GUID *PNAME##Ptr = &PNAME; \
  if (UuidFromStringA(reinterpret_cast<unsigned char *>(Tcl_GetStringFromObj(objv[OBJNUM], NULL)), &PNAME) != RPC_S_OK) PNAME##Ptr = NULL;
#define FreeParam_TclToC_GUID(PNAME)

#define InitParam_TclToC_String(OBJNUM, PNAME) \
  wchar_t *PNAME = NULL; \
  Tcl_DString PNAME##DS; \
  Tcl_WinUtfToTChar(Tcl_GetStringFromObj(objv[OBJNUM], NULL), -1, &PNAME##DS); \
  if (Tcl_DStringLength(&PNAME##DS) != 0) PNAME = (wchar_t *)Tcl_DStringValue(&PNAME##DS);
#define FreeParam_TclToC_String(PNAME) \
  Tcl_DStringFree(&PNAME##DS);

#define AssignParam_TclToC_WideInt(OBJNUM, PNAME) \
  Tcl_GetWideIntFromObj(interp, objv[OBJNUM], (Tcl_WideInt *)&PNAME);
#define InitParam_TclToC_WideInt(OBJNUM, PNAME) \
  Tcl_WideInt PNAME; \
  AssignParam_TclToC_WideInt(OBJNUM, PNAME);
#define FreeParam_TclToC_WideInt(PNAME)

#define AssignParam_TclToC_Int(OBJNUM, PNAME) \
  Tcl_GetIntFromObj(interp, objv[OBJNUM], (int *)&PNAME);
#define InitParam_TclToC_Int(OBJNUM, PNAME) \
  int PNAME; \
  AssignParam_TclToC_Int(OBJNUM, PNAME);
#define FreeParam_TclToC_Int(PNAME)

#define InitParam_TclToC_Boolean(OBJNUM, PNAME) \
  int PNAME; \
  Tcl_GetBooleanFromObj(interp, objv[OBJNUM], &PNAME);
#define FreeParam_TclToC_Boolean(PNAME)

#define InitParam_TclToC_ByteArray(OBJNUM, PNAME) \
  void *PNAME; \
  PNAME = Tcl_GetByteArrayFromObj(objv[OBJNUM], NULL);
#define FreeParam_TclToC_ByteArray(PNAME)

#define InitParam_TclToC_CustomType(OBJNUM, PNAME, TYPE) \
  TYPE *PNAME; \
  PNAME = (TYPE *)Tcl_GetByteArrayFromObj(objv[OBJNUM], NULL);
#define FreeParam_TclToC_CustomType(PNAME)

#define InitParam_TclToC_Pointer(OBJNUM, PNAME, TYPE) \
  TYPE PNAME = nullptr; \
  sscanf_s(Tcl_GetStringFromObj(objv[OBJNUM], NULL), "ptr%p", &PNAME);
#define FreeParam_TclToC_Pointer(PNAME)

/* Set result C to Tcl Obj for list */

#define InitResult_CToTcl() \
  Tcl_Obj *__result__ = Tcl_NewListObj(0, NULL);
#define AppendResult_CToTcl_Int(VALUE) \
  Tcl_ListObjAppendElement(interp, __result__, Tcl_NewIntObj(VALUE));
#define AppendResult_CToTcl_ByteArray(VALUE, VALUE_LENGTH) \
  Tcl_ListObjAppendElement(interp, __result__, Tcl_NewByteArrayObj(VALUE, VALUE_LENGTH));
#define AppendResult_CToTcl_CustomType(VALUE, TYPE) \
  Tcl_ListObjAppendElement(interp, __result__, Tcl_NewByteArrayObj((const unsigned char *)VALUE, sizeof(TYPE)));
#define AppendResult_CToTcl_Pointer(VALUE) \
  char buf[POINTER_STR_LENGTH+4]; \
  _snprintf_s(buf, POINTER_STR_LENGTH+4, POINTER_STR_LENGTH+3, "ptr%p", VALUE); \
  Tcl_ListObjAppendElement(interp, __result__, Tcl_NewStringObj(buf, -1));
#define AppendResult_CToTcl_String(VALUE) \
  Tcl_DString VALUE##ResultDS; \
  Tcl_WinTCharToUtf(VALUE, -1, &VALUE##ResultDS); \
  Tcl_ListObjAppendElement(interp, __result__, Tcl_NewStringObj(Tcl_DStringValue(&VALUE##ResultDS), -1)); \
  Tcl_DStringFree(&VALUE##ResultDS);
#define AppendResult_CToTcl_Object(VALUE) \
  Tcl_ListObjAppendElement(interp, __result__, VALUE);
#define SetResult_CToTcl() \
  Tcl_SetObjResult(interp, __result__);

/* Set result C to Tcl Obj for dict */

#define InitResultDict_CToTcl() \
  Tcl_Obj *__result__ = Tcl_NewDictObj();
#define AppendResultDict_CToTcl_Int(NAME, VALUE) \
  Tcl_DictObjPut(interp, __result__, Tcl_NewStringObj(NAME, -1), Tcl_NewIntObj(VALUE));
#define AppendResultDict_CToTcl_Boolean(NAME, VALUE) \
  Tcl_DictObjPut(interp, __result__, Tcl_NewStringObj(NAME, -1), Tcl_NewBooleanObj(VALUE));
#define AppendResultDict_CToTcl_Unicode(NAME, VALUE, LENGTH) \
  Tcl_DictObjPut(interp, __result__, Tcl_NewStringObj(NAME, -1), Tcl_NewUnicodeObj(VALUE, LENGTH));
#define AppendResultDict_CToTcl_WideInt(NAME, VALUE) \
  Tcl_DictObjPut(interp, __result__, Tcl_NewStringObj(NAME, -1), Tcl_NewWideIntObj(VALUE));
#define AppendResultDict_CToTcl_Object(NAME, VALUE) \
  Tcl_DictObjPut(interp, __result__, Tcl_NewStringObj(NAME, -1), VALUE);
#define AppendResultDict_CToTcl_GUID(NAME, VALUE) \
  Tcl_DictObjPut(interp, __result__, Tcl_NewStringObj(NAME, -1), Tcl_NewGuidObj(VALUE));
#define AppendResultDict_CToTcl_StringConst(NAME, VALUE) \
  Tcl_DictObjPut(interp, __result__, Tcl_NewStringObj(NAME, -1), Tcl_NewStringObj(VALUE, -1));
#define AppendResultDict_CToTcl_CustomType(NAME, VALUE, TYPE) \
  Tcl_DictObjPut(interp, __result__, Tcl_NewStringObj(NAME, -1), Tcl_NewByteArrayObj((const unsigned char *)VALUE, sizeof(TYPE)));

#define SetResultDict_CToTcl() \
  Tcl_SetObjResult(interp, __result__);

/* Set result C to Tcl Obj */

#define SetResult_CToTcl_String(VALUE); \
  Tcl_DString bufDS; \
  Tcl_WinTCharToUtf(VALUE, -1, &bufDS); \
  Tcl_SetObjResult(interp, Tcl_NewStringObj(Tcl_DStringValue(&bufDS), -1)); \
  Tcl_DStringFree(&bufDS);

#define SetResult_CToTcl_Int(VALUE); \
  Tcl_SetObjResult(interp, Tcl_NewIntObj(VALUE));

#define SetResult_CToTcl_Boolean(VALUE); \
  Tcl_SetObjResult(interp, Tcl_NewIntObj(VALUE));

#define SetResult_CToTcl_CustomType(VALUE, TYPE); \
  Tcl_SetObjResult(interp, Tcl_NewByteArrayObj((const unsigned char *)VALUE, sizeof(TYPE)));

#define SetResult_CToTcl_Pointer(VALUE) \
  char buf[POINTER_STR_LENGTH+4]; \
  _snprintf_s(buf, POINTER_STR_LENGTH+4, POINTER_STR_LENGTH+3, "ptr%p", VALUE); \
  Tcl_SetObjResult(interp, Tcl_NewStringObj(buf, -1));

#define SetResult_CToTcl_Object(VALUE) \
  Tcl_SetObjResult(interp, VALUE);


Tcl_Obj *Tcl_NewGuidObj(const GUID& Guid)
{
  wchar_t guidstr[37];
  FSF.sprintf(guidstr,L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",Guid.Data1,Guid.Data2,Guid.Data3,Guid.Data4[0],Guid.Data4[1],Guid.Data4[2],Guid.Data4[3],Guid.Data4[4],Guid.Data4[5],Guid.Data4[6],Guid.Data4[7]);
  Tcl_DString ds;
  Tcl_WinTCharToUtf(guidstr, -1, &ds);
  Tcl_Obj *result = Tcl_NewStringObj(Tcl_DStringValue(&ds), -1);
  Tcl_DStringFree(&ds);
  return result;
};

Tcl_Obj *Tcl_NewWStringObj(const wchar_t *str)
{
  Tcl_DString ds;
  Tcl_WinTCharToUtf(str, -1, &ds);
  Tcl_Obj *result = Tcl_NewStringObj(Tcl_DStringValue(&ds), -1);
  Tcl_DStringFree(&ds);
  return result;
};
