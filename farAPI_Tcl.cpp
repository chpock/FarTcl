
#define POINTER_STR_LENGTH 50

/* Convert Tcl Obj to C, Free converted values */

#define InitParam(TYPE, NAME, OBJNUM) \
  InitParam_TclToC_##TYPE(OBJNUM, NAME)
#define FreeParam(TYPE, NAME) \
  FreeParam_TclToC_##TYPE(NAME)
#define AssignParam(TYPE, NAME, OBJNUM) \
  AssignParam_TclToC_##TYPE(OBJNUM, NAME)

#define AssignParam_TclToC_GUID(OBJNUM, PNAME) \
  UuidFromStringA(reinterpret_cast<unsigned char *>(Tcl_GetStringFromObj(objv[OBJNUM], nullptr)), &PNAME);
#define InitParam_TclToC_GUID(OBJNUM, PNAME) \
  GUID PNAME##Static; \
  GUID *PNAME = &PNAME##Static; \
  if (UuidFromStringA(reinterpret_cast<unsigned char *>(Tcl_GetStringFromObj(objv[OBJNUM], nullptr)), &PNAME##Static) != RPC_S_OK) PNAME = nullptr;
#define FreeParam_TclToC_GUID(PNAME)

#define UNIQ(__PREFIX) __UNIQ(__PREFIX, __LINE__)
#define __UNIQ(x, y) ___UNIQ(x, y)
#define ___UNIQ(x, y) x##y

#define AssignParam_TclToC_String(OBJNUM, PNAME) \
  Tcl_DString UNIQ(__DS__); \
  Tcl_DStringInit(&UNIQ(__DS__)); \
  Tcl_WinUtfToTChar(Tcl_GetStringFromObj(objv[OBJNUM], nullptr), -1, &UNIQ(__DS__)); \
  if (Tcl_DStringLength(&UNIQ(__DS__)) != 0) { \
      PNAME = (wchar_t *)ckalloc(Tcl_DStringLength(&UNIQ(__DS__)) + sizeof(wchar_t)); \
      memcpy(PNAME, Tcl_DStringValue(&UNIQ(__DS__)), (size_t)Tcl_DStringLength(&UNIQ(__DS__))); \
      PNAME[(size_t)Tcl_DStringLength(&UNIQ(__DS__))/sizeof(wchar_t)] = L'\0'; \
  } else { \
      PNAME = nullptr; \
  } \
  Tcl_DStringFree(&UNIQ(__DS__))
#define InitParam_TclToC_String(OBJNUM, PNAME) \
  wchar_t *PNAME; \
  AssignParam_TclToC_String(OBJNUM, PNAME)
#define FreeParam_TclToC_String(PNAME) \
  ckfree(PNAME)

#define AssignParam_TclToC_StringConst(OBJNUM, PNAME) \
  wchar_t *UNIQ(__STR__); \
  Tcl_DString UNIQ(__DS__); \
  Tcl_DStringInit(&UNIQ(__DS__)); \
  Tcl_WinUtfToTChar(Tcl_GetStringFromObj(objv[OBJNUM], nullptr), -1, &UNIQ(__DS__)); \
  if (Tcl_DStringLength(&UNIQ(__DS__)) != 0) { \
      UNIQ(__STR__) = (wchar_t *)ckalloc(Tcl_DStringLength(&UNIQ(__DS__)) + sizeof(wchar_t)); \
      memcpy(UNIQ(__STR__), Tcl_DStringValue(&UNIQ(__DS__)), (size_t)Tcl_DStringLength(&UNIQ(__DS__))); \
      UNIQ(__STR__)[(size_t)Tcl_DStringLength(&UNIQ(__DS__))/sizeof(wchar_t)] = L'\0'; \
      PNAME = UNIQ(__STR__); \
  } else { \
      PNAME = nullptr; \
  } \
  Tcl_DStringFree(&UNIQ(__DS__))
#define FreeParam_TclToC_StringConst(PNAME) \
  ckfree(PNAME)

#define AssignParam_TclToC_StringLength(OBJNUM, PNAME) \
  Tcl_DString UNIQ(__DS__); \
  Tcl_DStringInit (&UNIQ(__DS__)); \
  Tcl_WinUtfToTChar(Tcl_GetStringFromObj(objv[OBJNUM], nullptr), -1, &UNIQ(__DS__)); \
  PNAME = Tcl_DStringLength(&UNIQ(__DS__))/sizeof(wchar_t); \
  Tcl_DStringFree(&UNIQ(__DS__))
#define InitParam_TclToC_StringLength(OBJNUM, PNAME) \
  size_t PNAME; \
  AssignParam_TclToC_StringLength(OBJNUM, PNAME)
#define FreeParam_TclToC_StringLength(PNAME)

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

#define InitParam_TclToC_Pointer(OBJNUM, PNAME, TYPE) \
  TYPE PNAME = nullptr; \
  sscanf_s(Tcl_GetStringFromObj(objv[OBJNUM], nullptr), "ptr%p", &PNAME);
#define FreeParam_TclToC_Pointer(PNAME)

/* Pointer aliases */

#define AssignParam_TclToC_HANDLE(OBJNUM, NAME) \
  AssignParam_TclToC_Pointer(OBJNUM, NAME)
#define InitParam_TclToC_HANDLE(OBJNUM, NAME) \
  InitParam_TclToC_Pointer(OBJNUM, NAME, HANDLE)
#define FreeParam_TclToC_HANDLE(NAME) \
  FreeParam_TclToC_Pointer(NAME)

/* Shorts */

#define AssignParam_TclToC_Wide(OBJNUM, PNAME) \
  AssignParam_TclToC_WideInt(OBJNUM, PNAME)
#define InitParam_TclToC_Wide(OBJNUM, PNAME) \
  InitParam_TclToC_WideInt(OBJNUM, PNAME)
#define FreeParam_TclToC_Wide(PNAME) \
  FreeParam_TclToC_WideInt(PNAME)

#define AssignParam_TclToC_UWide(OBJNUM, PNAME) \
  AssignParam_TclToC_WideInt(OBJNUM, PNAME)
#define InitParam_TclToC_UWide(OBJNUM, PNAME) \
  InitParam_TclToC_WideInt(OBJNUM, PNAME)
#define FreeParam_TclToC_UWide(PNAME) \
  FreeParam_TclToC_WideInt(PNAME)

#define AssignParam_TclToC_Str(OBJNUM, PNAME) \
  AssignParam_TclToC_String(OBJNUM, PNAME)
#define InitParam_TclToC_Str(OBJNUM, PNAME) \
  InitParam_TclToC_String(OBJNUM, PNAME)
#define FreeParam_TclToC_Str(PNAME) \
  FreeParam_TclToC_String(PNAME)

#define AssignParam_TclToC_StrConst(OBJNUM, PNAME) \
  AssignParam_TclToC_StringConst(OBJNUM, PNAME)
#define InitParam_TclToC_StrConst(OBJNUM, PNAME) \
  InitParam_TclToC_StringConst(OBJNUM, PNAME)
#define FreeParam_TclToC_StrConst(PNAME) \
  FreeParam_TclToC_StringConst(PNAME)

#define AssignParam_TclToC_StrLen(OBJNUM, PNAME) \
  AssignParam_TclToC_StringLength(OBJNUM, PNAME)
#define InitParam_TclToC_StrLen(OBJNUM, PNAME) \
  InitParam_TclToC_StringLength(OBJNUM, PNAME)
#define FreeParam_TclToC_StrLen(PNAME) \
  FreeParam_TclToC_StringLength(PNAME)

#define AssignParam_TclToC_Ptr(OBJNUM, PNAME) \
  AssignParam_TclToC_Pointer(OBJNUM, PNAME)
#define InitParam_TclToC_Ptr(OBJNUM, PNAME) \
  InitParam_TclToC_Pointer(OBJNUM, PNAME)
#define FreeParam_TclToC_Ptr(PNAME) \
  FreeParam_TclToC_Pointer(PNAME)

#define AssignParam_TclToC_Bool(OBJNUM, PNAME) \
  AssignParam_TclToC_Boolean(OBJNUM, PNAME)
#define InitParam_TclToC_Bool(OBJNUM, PNAME) \
  InitParam_TclToC_Boolean(OBJNUM, PNAME)
#define FreeParam_TclToC_Bool(PNAME) \
  FreeParam_TclToC_Boolean(PNAME)

/* Set result C to Tcl Obj */

#define ReturnResult(TYPE, NAME, CODE) \
  SetResult(TYPE, NAME); \
  return TCL_##CODE

#define _GET_NTH_ARG(_1, _2, _3, N, ...) N
#define Return(...) _GET_NTH_ARG("", ##__VA_ARGS__, Return2, Return1, Return0)(__VA_ARGS__)

#define Return0() \
  return __return_code
#define Return1(CODE) \
  return TCL_##CODE

#define SetResultError(MESSAGE) \
  SetReturnCode(ERROR); \
  InitResult(Str, MESSAGE)

#define InitReturnCode() \
  int __return_code = TCL_OK

#define SetReturnCode(CODE) \
  __return_code = TCL_##CODE

#define SetResult(TYPE, NAME) \
  InitResult_CToTcl_2##TYPE(NAME)

#define InitResult(...) _GET_NTH_ARG("", ##__VA_ARGS__, InitResult2, InitResult1, InitResult0)(__VA_ARGS__)

#define InitResult1(TYPE) \
  InitResult_CToTcl_1##TYPE(); \
  Tcl_SetObjResult(interp, Result)
#define InitResult2(TYPE, VALUE) \
  InitResult_CToTcl_2##TYPE(VALUE)

#define InitResult_CToTcl_1Dict(); \
  Tcl_Obj *Result = Tcl_NewDictObj()
#define InitResult_CToTcl_1List(); \
  Tcl_Obj *Result = Tcl_NewListObj(0, nullptr)

#define InitResult_CToTcl_2Str(VALUE) \
  Tcl_DString bufDS; \
  Tcl_WinTCharToUtf(VALUE, -1, &bufDS); \
  Tcl_SetObjResult(interp, Tcl_NewStringObj(Tcl_DStringValue(&bufDS), -1)); \
  Tcl_DStringFree(&bufDS)
#define InitResult_CToTcl_2Pointer(VALUE) \
  char buf[POINTER_STR_LENGTH+4]; \
  _snprintf_s(buf, POINTER_STR_LENGTH+4, POINTER_STR_LENGTH+3, "ptr%p", VALUE); \
  Tcl_SetObjResult(interp, Tcl_NewStringObj(buf, -1))
#define InitResult_CToTcl_2HANDLE(VALUE) \
  InitResult_CToTcl_2Pointer(VALUE)
#define InitResult_CToTcl_2Int(VALUE) \
  Tcl_SetObjResult(interp, Tcl_NewIntObj(VALUE))

/* other */
#define NewDString(NAME) \
  Tcl_DString NAME; \
  Tcl_DStringInit(&NAME)
#define FreeDString(NAME) \
  Tcl_DStringFree(&NAME)
#define APICall(NAME) \
  int Far_##NAME(ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
#define NewStruct(TYPE, NAME) \
  TYPE NAME = {sizeof(TYPE)}; \
  NAME.StructSize = sizeof(TYPE)
#define NewDict(NAME) \
  Tcl_Obj *NAME = Tcl_NewDictObj()
#define NewList(NAME) \
  Tcl_Obj *NAME = Tcl_NewListObj(0, nullptr)

#define LAppend(NAME, TYPE, VAR) \
  LAppend_##TYPE(NAME, VAR)
#define LAppend_GUID(NAME, VAR) \
  Tcl_ListObjAppendElement(interp, NAME, Tcl_NewGuidObj(VAR))
#define LAppend_String(NAME, VAR) \
  Tcl_ListObjAppendElement(interp, NAME, Tcl_NewWStringObj(VAR))
#define LAppend_Int(NAME, VAR) \
  Tcl_ListObjAppendElement(interp, NAME, Tcl_NewIntObj(VAR))
#define LAppend_Wide(NAME, VAR) \
  Tcl_ListObjAppendElement(interp, NAME, Tcl_NewWideIntObj(VAR))
#define LAppend_Object(NAME, VAR) \
  Tcl_ListObjAppendElement(interp, NAME, VAR);
#define LAppend_Ptr(NAME, VAR) \
  char buf[POINTER_STR_LENGTH+4]; \
  _snprintf_s(buf, POINTER_STR_LENGTH+4, POINTER_STR_LENGTH+3, "ptr%p", VAR); \
  Tcl_ListObjAppendElement(interp, NAME, Tcl_NewStringObj(buf, -1))

#define DSet(NAME, KEY, TYPE, VAR) \
  DSet_##TYPE(NAME, KEY, VAR)
#define DSet_Int(NAME, KEY, VAR) \
  Tcl_DictObjPut(interp, NAME, Tcl_NewStringObj(KEY, -1), Tcl_NewIntObj(VAR))
#define DSet_Wide(NAME, KEY, VAR) \
  Tcl_DictObjPut(interp, NAME, Tcl_NewStringObj(KEY, -1), Tcl_NewWideIntObj(VAR))
#define DSet_String(NAME, KEY, VAR) \
  Tcl_DictObjPut(interp, NAME, Tcl_NewStringObj(KEY, -1), Tcl_NewWStringObj(VAR))
#define DSet_Object(NAME, KEY, VAR) \
  Tcl_DictObjPut(interp, NAME, Tcl_NewStringObj(KEY, -1), VAR)
#define DSet_GUID(NAME, KEY, VAR) \
  Tcl_DictObjPut(interp, NAME, Tcl_NewStringObj(KEY, -1), Tcl_NewGuidObj(VAR))
#define DSet_Boolean(NAME, KEY, VAR) \
  Tcl_DictObjPut(interp, NAME, Tcl_NewStringObj(KEY, -1), Tcl_NewBooleanObj(VAR))
#define DSetResult(KEY, TYPE, VAR) \
  DSet(__result__, KEY, TYPE, VAR)

/* aliases */

#define DSet_UWide(NAME, KEY, VAR) \
  DSet_Wide(NAME, KEY, VAR)
#define DSet_Str(NAME, KEY, VAR) \
  DSet_String(NAME, KEY, VAR)
#define DSet_Obj(NAME, KEY, VAR) \
  DSet_Object(NAME, KEY, VAR)
#define DSet_Bool(NAME, KEY, VAR) \
  DSet_Boolean(NAME, KEY, VAR)

#define LAppend_UWide(NAME, VAR) \
  LAppend_Wide(NAME, VAR)
#define LAppend_Str(NAME, VAR) \
  LAppend_String(NAME, VAR)
#define LAppend_Obj(NAME, VAR) \
  LAppend_Object(NAME, VAR)


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

int Tcl_GetUWideIntFromObj(Tcl_Interp *interp, Tcl_Obj *objPtr,
	    unsigned __int64 *widePtr)
{
  Tcl_WideInt tmp;
  Tcl_GetWideIntFromObj(interp, objPtr, &tmp);
  *widePtr = tmp;
};

/* Set result C to Tcl Obj for list */

/* obsolete start
#define InitParam_TclToC_ByteArray(OBJNUM, PNAME) \
  void *PNAME; \
  PNAME = Tcl_GetByteArrayFromObj(objv[OBJNUM], NULL);
#define FreeParam_TclToC_ByteArray(PNAME)

 #define InitParam_TclToC_CustomType(OBJNUM, PNAME, TYPE) \
  TYPE *PNAME; \
  PNAME = (TYPE *)Tcl_GetByteArrayFromObj(objv[OBJNUM], NULL);
#define FreeParam_TclToC_CustomType(PNAME)

#define InitResult_CToTcl_List() \
  Tcl_Obj *__result__ = Tcl_NewListObj(0, nullptr)
#define LAppendResult(TYPE, NAME) \
  AppendResult_CToTcl_##TYPE(NAME)

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

#define AppendResult_CToTcl_Str(VALUE) \
  AppendResult_CToTcl_String(VALUE)
#define AppendResult_CToTcl_Ptr(VALUE) \
  AppendResult_CToTcl_Pointer(VALUE)
obsolete end */

/* Set result C to Tcl Obj for dict

/* obsolete start
#define InitResult_CToTcl_Dict() \
  Tcl_Obj *__result__ = Tcl_NewDictObj()
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

/* obsolete start
#define SetResult_CToTcl_String(VALUE) \
  Tcl_DString bufDS; \
  Tcl_WinTCharToUtf(VALUE, -1, &bufDS); \
  Tcl_SetObjResult(interp, Tcl_NewStringObj(Tcl_DStringValue(&bufDS), -1)); \
  Tcl_DStringFree(&bufDS);

#define SetResult_CToTcl_Int(VALUE) \
  Tcl_SetObjResult(interp, Tcl_NewIntObj(VALUE));

#define SetResult_CToTcl_Boolean(VALUE) \
  Tcl_SetObjResult(interp, Tcl_NewIntObj(VALUE));

#define SetResult_CToTcl_CustomType(VALUE, TYPE) \
  Tcl_SetObjResult(interp, Tcl_NewByteArrayObj((const unsigned char *)VALUE, sizeof(TYPE)));

#define SetResult_CToTcl_Pointer(VALUE) \
  char buf[POINTER_STR_LENGTH+4]; \
  _snprintf_s(buf, POINTER_STR_LENGTH+4, POINTER_STR_LENGTH+3, "ptr%p", VALUE); \
  Tcl_SetObjResult(interp, Tcl_NewStringObj(buf, -1));

#define SetResult_CToTcl_Object(VALUE) \
  Tcl_SetObjResult(interp, VALUE);

/* Shorts
#define SetResult_CToTcl_Str(VALUE) \
  SetResult_CToTcl_String(VALUE)

#define SetResult_CToTcl_Ptr(VALUE) \
  SetResult_CToTcl_Pointer(VALUE)

/* obsolete end */
