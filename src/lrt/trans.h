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

#ifndef EBBOS_LRT_TRANS_H
#define EBBOS_LRT_TRANS_H

#include <cstdint>

#include <lrt/arch/trans.h>
#include <arch/args.h>

namespace ebbos {
  namespace lrt {
    namespace trans {
      void init(int nCores);
      void initCpu();

      class EbbRep {
      public:
      };

      class FuncRet {
      public:
        union {
          void (*func)();
          uintptr_t ret;
        };
      };

      class Manager : public EbbRep {
      public:
        virtual bool preCall(Args* args, int fnum, FuncRet* fret)=0;
        virtual uintptr_t postCall(uintptr_t ret)=0;
        virtual void bind()=0;
      };

      class LocalEntry {
      public:
        LocalEntry();
        EbbRep* ref;
        void (*(*rep)[256])(); // pointer to a size 256 array of
                               // pointers to functions taking no
                               // arguments and having no return
      };

      extern LocalEntry* localTable;
      const int LTABLE_SIZE = 1 << 21; //2 MB

      const int N_LOCAL_ENTRIES = LTABLE_SIZE / sizeof(LocalEntry);

      class RootManager : public Manager {
      public:
        virtual bool preCall(Args* args, int fnum, FuncRet* fret);
        virtual uintptr_t postCall(uintptr_t ret);
        virtual void bind();
        virtual void addRep(LocalEntry* l, EbbRep* rep);
      private:
        EbbRep* repTbl[N_LOCAL_ENTRIES];
      };
      extern RootManager* rootManager;

      template <class T>
      class Ebb;

      class GlobalEntry {
      public:
        GlobalEntry() : mgr_(0), padding(0) {}
        explicit GlobalEntry(void* mgr) : mgr_(mgr) {}
        void* mgr_;
        uintptr_t padding;
      };

      template <class T>
      class Ebb {
      public:
        Ebb();
        explicit Ebb(T* rep);
        //hack to prevent this from being used over the implicit copy
        //constructor when we instantiate an EbbManager Ebb
        template<int = 0>
        explicit Ebb(Ebb<Manager> mgr);
        //placement constructors
        explicit Ebb(LocalEntry* l, T* rep = nullptr) : le(l) {
          if (rep != nullptr) {
            rootManager->addRep(l, rep);
            uintptr_t loc = reinterpret_cast<uintptr_t>(le);
            uintptr_t index = (loc - LOCAL_MEM_VIRT) / sizeof(LocalEntry);
            uintptr_t g = GLOBAL_MEM_VIRT + (index * sizeof(GlobalEntry));
            GlobalEntry* ge = reinterpret_cast<GlobalEntry*>(g);
            ge->mgr_ = static_cast<void*>(&rootManager);
          }
        }
        Ebb(LocalEntry* l, Ebb<Manager> mgr) : le(l) {
          uintptr_t loc = reinterpret_cast<uintptr_t>(le);
          uintptr_t index = (loc - LOCAL_MEM_VIRT) / sizeof(LocalEntry);
          uintptr_t g = GLOBAL_MEM_VIRT + (index * sizeof(GlobalEntry));
          GlobalEntry* ge = reinterpret_cast<GlobalEntry*>(g);
          *ge = GlobalEntry(mgr);
        }
        ~Ebb() {}
        template <class R>
        operator Ebb<R>() {
          return Ebb<R>(le);
        }
        operator LocalEntry*() {
          return le;
        }
        void bind(Ebb<Manager> mgr);
        T* operator->() const {
          return reinterpret_cast<T*>(le->ref);
        }
        T operator*() const {
          return *reinterpret_cast<T*>(le->ref);
        }
        operator void*() {
          return reinterpret_cast<void*>(le);
        }
      private:
        LocalEntry* const le;
      };
      extern Ebb<Manager> rootManagerEbb;
    }
  }
}

#endif
