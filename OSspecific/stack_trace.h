// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_STACK_TRACE_H_
#define BASE_DEBUG_STACK_TRACE_H_

#include <stddef.h>

#include <iosfwd>
#include <string> 

 
#include <unistd.h>
 

#if defined(WIN32)
struct _EXCEPTION_POINTERS;
struct _CONTEXT;
#endif

 
// Enables stack dump to console output on exception and signals.
// When enabled, the process will quit immediately. This is meant to be used in
// unit_tests only! This is not thread-safe: only call from main thread.
// In sandboxed processes, this has to be called before the sandbox is turned
// on.
// Calling this function on Linux opens /proc/self/maps and caches its
// contents. In non-official builds, this function also opens the object files
// that are loaded in memory and caches their file descriptors (this cannot be
// done in official builds because it has security implications).
 bool EnableInProcessStackDumping();

 
 void SetStackDumpFirstChanceCallback(bool (*handler)(int,
                                                                 void*,
                                                                 void*));
 

// Returns end of the stack, or 0 if we couldn't get it.
 
 uintptr_t GetStackEnd();
 

// A stacktrace can be helpful in debugging. For example, you can include a
// stacktrace member in a object (probably around #ifndef NDEBUG) so that you
// can later see where the given object was created from.
class   StackTrace {
 public:
  // Creates a stacktrace from the current location.
  StackTrace();

  // Creates a stacktrace from the current location, of up to |count| entries.
  // |count| will be limited to at most |kMaxTraces|.
  explicit StackTrace(size_t count);

  // Creates a stacktrace from an existing array of instruction
  // pointers (such as returned by Addresses()).  |count| will be
  // limited to at most |kMaxTraces|.
  StackTrace(const void* const* trace, size_t count);

#if defined(_WIN32)
  // Creates a stacktrace for an exception.
  // Note: this function will throw an import not found (StackWalk64) exception
  // on system without dbghelp 5.1.
  StackTrace(_EXCEPTION_POINTERS* exception_pointers);
  StackTrace(const _CONTEXT* context);
#endif

  // Copying and assignment are allowed with the default functions.

  // Gets an array of instruction pointer values. |*count| will be set to the
  // number of elements in the returned array.
  const void* const* Addresses(size_t* count) const;

  // Prints the stack trace to stderr.
  void Print() const;

#if !defined(__UCLIBC__) & !defined(_AIX)
  // Resolves backtrace to symbols and write to stream.
  void OutputToStream(std::ostream* os) const;
#endif

  // Resolves backtrace to symbols and returns as string.
  std::string ToString() const;

 private:
#if defined(_WIN32)
  void InitTrace(const _CONTEXT* context_record);
#endif

  // From http://msdn.microsoft.com/en-us/library/bb204633.aspx,
  // the sum of FramesToSkip and FramesToCapture must be less than 63,
  // so set it to 62. Even if on POSIX it could be a larger value, it usually
  // doesn't give much more information.
  static const int kMaxTraces = 62;

  void* trace_[kMaxTraces];

  // The number of valid frames in |trace_|.
  size_t count_;
};

 
// Traces the stack by using frame pointers. This function is faster but less
// reliable than StackTrace. It should work for debug and profiling builds,
// but not for release builds (although there are some exceptions).
//
// Writes at most |max_depth| frames (instruction pointers) into |out_trace|
// after skipping |skip_initial| frames. Note that the function itself is not
// added to the trace so |skip_initial| should be 0 in most cases.
// Returns number of frames written.
 size_t TraceStackFramePointers(const void** out_trace,
                                           size_t max_depth,
                                           size_t skip_initial);

// Links stack frame |fp| to |parent_fp|, so that during stack unwinding
// TraceStackFramePointers() visits |parent_fp| after visiting |fp|.
// Both frame pointers must come from __builtin_frame_address().
// Destructor restores original linkage of |fp| to avoid corrupting caller's
// frame register on return.
//
// This class can be used to repair broken stack frame chain in cases
// when execution flow goes into code built without frame pointers:
//
// void DoWork() {
//   Call_SomeLibrary();
// }
// static __thread void*  g_saved_fp;
// void Call_SomeLibrary() {
//   g_saved_fp = __builtin_frame_address(0);
//   some_library_call(...); // indirectly calls SomeLibrary_Callback()
// }
// void SomeLibrary_Callback() {
//   ScopedStackFrameLinker linker(__builtin_frame_address(0), g_saved_fp);
//   ...
//   TraceStackFramePointers(...);
// }
//
// This produces the following trace:
//
// #0 SomeLibrary_Callback()
// #1 <address of the code inside SomeLibrary that called #0>
// #2 DoWork()
// ...rest of the trace...
//
// SomeLibrary doesn't use frame pointers, so when SomeLibrary_Callback()
// is called, stack frame register contains bogus value that becomes callback'
// parent frame address. Without ScopedStackFrameLinker unwinding would've
// stopped at that bogus frame address yielding just two first frames (#0, #1).
// ScopedStackFrameLinker overwrites callback's parent frame address with
// Call_SomeLibrary's frame, so unwinder produces full trace without even
// noticing that stack frame chain was broken.
 
 
#endif  // BASE_DEBUG_STACK_TRACE_H_
