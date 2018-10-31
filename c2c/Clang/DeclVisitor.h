//===- DeclVisitor.h - Visitor for Decl subclasses --------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines the DeclVisitor interface.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_AST_DECLVISITOR_H
#define LLVM_CLANG_AST_DECLVISITOR_H

#include "Clang/Decl.h"
#include "Clang/DeclBase.h"
#include "Clang/DeclCXX.h"
#include "Clang/DeclFriend.h"
#include "Clang/DeclObjC.h"
#include "Clang/DeclOpenMP.h"
#include "Clang/DeclTemplate.h"
#include <llvm/Support/ErrorHandling.h>

namespace c2lang {

namespace declvisitor {

template <typename T> struct make_ptr { using type = T *; };
template <typename T> struct make_const_ptr { using type = const T *; };

/// A simple visitor class that helps create declaration visitors.
template<template <typename> class Ptr, typename ImplClass, typename RetTy=void>
class Base {
public:
#define PTR(CLASS) typename Ptr<CLASS>::type
#define DISPATCH(NAME, CLASS) \
  return static_cast<ImplClass*>(this)->Visit##NAME(static_cast<PTR(CLASS)>(D))

  RetTy Visit(PTR(Decl) D) {
    switch (D->getKind()) {
#define DECL(DERIVED, BASE) \
      case Decl::DERIVED: DISPATCH(DERIVED##Decl, DERIVED##Decl);
#define ABSTRACT_DECL(DECL)
#include "Clang/DeclNodes.inc"
    }
    llvm_unreachable("Decl that isn't part of DeclNodes.inc!");
  }

  // If the implementation chooses not to implement a certain visit
  // method, fall back to the parent.
#define DECL(DERIVED, BASE) \
  RetTy Visit##DERIVED##Decl(PTR(DERIVED##Decl) D) { DISPATCH(BASE, BASE); }
#include "Clang/DeclNodes.inc"

  RetTy VisitDecl(PTR(Decl) D) { return RetTy(); }

#undef PTR
#undef DISPATCH
};

} // namespace declvisitor

/// A simple visitor class that helps create declaration visitors.
///
/// This class does not preserve constness of Decl pointers (see also
/// ConstDeclVisitor).
template<typename ImplClass, typename RetTy = void>
class DeclVisitor
 : public declvisitor::Base<declvisitor::make_ptr, ImplClass, RetTy> {};

/// A simple visitor class that helps create declaration visitors.
///
/// This class preserves constness of Decl pointers (see also DeclVisitor).
template<typename ImplClass, typename RetTy = void>
class ConstDeclVisitor
 : public declvisitor::Base<declvisitor::make_const_ptr, ImplClass, RetTy> {};

} // namespace c2lang

#endif // LLVM_CLANG_AST_DECLVISITOR_H
