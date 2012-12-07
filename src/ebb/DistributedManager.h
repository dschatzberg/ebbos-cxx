/*
 * Copyright (C) 2012 by Project SESA, Boston University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EBBOS_EBB_DISTRIBUTEDMANAGER_H
#define EBBOS_EBB_DISTRIBUTEDMANAGER_H

#include <ebb/ebb.h>
#include <ebb/MemAllocator/MemAllocator.h>
#include <sync/Spinlock.h>

namespace ebbos {
  template <class T>
  class DistributedManager : public Manager {
  public:
    DistributedManager()
    {
    }

    virtual bool preCall(Args* args, int fnum,
                         ebbos::lrt::trans::FuncRet* fret)
    {
      using lrt::trans::LocalEntry;
      void* rep = *reinterpret_cast<void**>(args);
      uintptr_t addr = reinterpret_cast<uintptr_t>(rep) - sizeof(uintptr_t);
      LocalEntry* le = reinterpret_cast<LocalEntry*>
        (reinterpret_cast<void*>(addr));

      l.lock();
      T* ref = nullptr;
      for (Node* temp = list; temp != nullptr; temp = temp->next) {
        if (temp->location == getLocation()) {
          ref = temp->ref;
          break;
        }
      }

      if (ref == nullptr) {
        ref = new T;
        access_once(le->ref) = ref;
        Node *n = new Node(ref);
        n->next = list;
        list = n;
      } else {
        access_once(le->ref) = ref;
      }
      l.unlock();
      *reinterpret_cast<EbbRep**>(args) = reinterpret_cast<T*>(ref);
      void (*(**theRep)[256])() = reinterpret_cast<void (*(**)[256])()>(ref);
      fret->func = (**theRep)[fnum];
      return true;
    }
    virtual uintptr_t postCall(uintptr_t ret)
    {
      return ret;
    }
    virtual void bind()
    {

    }
  private:
    class Node {
    public:
      int location;
      T* ref;
      Node* next;
      Node (T* rep) : ref(rep) {}
      void* operator new(size_t size) {
        return memAllocator->malloc(size);
      }
    };
    Node* list;
    Spinlock l;
  };
}

#endif
