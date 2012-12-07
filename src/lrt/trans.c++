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

#include <new>

#include <lrt/event.h>
#include <lrt/mem.h>
#include <lrt/transImpl.h>
#include <sync/compiler.h>

namespace ebbos {
  namespace lrt {
    namespace trans {
      LocalEntry** physLocalEntries;
      RootManager* rootManager;
      LocalEntry* localTable = reinterpret_cast<LocalEntry*>(LOCAL_MEM_VIRT);

      void
      init(int nCores)
      {
        initArch(nCores);
        void* rootManagerMem = mem::malloc(sizeof(RootManager), 0);
        rootManager = new (rootManagerMem) RootManager;
      }

      void
      initCpu()
      {
        initCpuArch();
        for (int i = 0; i < N_LOCAL_ENTRIES; i++) {
          localTable[i].ref = reinterpret_cast<EbbRep*>
            (&localTable[i].rep);
          localTable[i].rep = &defaultVtable;
        }
      }

      LocalEntry::LocalEntry() : ref(reinterpret_cast<EbbRep*>(&rep)),
                                 rep(&defaultVtable)
      {
      }

      void
      RootManager::addRep(LocalEntry* l, EbbRep* rep)
      {
        int index = LOCAL_MEM_VIRT - reinterpret_cast<uintptr_t>(l);
        access_once(repTbl[index]) = rep;
      }

      bool
      RootManager::preCall(Args* args, int fnum, FuncRet* fret)
      {
        void* rep = *reinterpret_cast<void**>(args);
        uintptr_t addr = reinterpret_cast<uintptr_t>(rep) - sizeof(uintptr_t);
        LocalEntry* le = reinterpret_cast<LocalEntry*>
          (reinterpret_cast<void*>(addr));
        int index = LOCAL_MEM_VIRT - reinterpret_cast<uintptr_t>(le);
        EbbRep* ref = access_once(repTbl[index]);
        access_once(le->ref) = ref;
        *reinterpret_cast<EbbRep**>(args) = ref;
        void (*(**theRep)[256])() = reinterpret_cast<void (*(**)[256])()>(ref);
        fret->func = (**theRep)[fnum];
        return true;
      }

      uintptr_t
      RootManager::postCall(uintptr_t ret)
      {
        return ret;
      }

      void
      RootManager::bind()
      {
      }

      extern "C" bool precall(Args* args, int fnum, FuncRet* fret)
      {
        void* rep = *reinterpret_cast<void**>(args);
        LocalEntry* le = reinterpret_cast<LocalEntry*>
          (reinterpret_cast<uintptr_t*>(rep) - 1);
        uintptr_t loc = reinterpret_cast<uintptr_t>(le);
        uintptr_t index = (loc - LOCAL_MEM_VIRT) / sizeof(LocalEntry);
        uintptr_t g = GLOBAL_MEM_VIRT + (index * sizeof(GlobalEntry));
        GlobalEntry* ge = reinterpret_cast<GlobalEntry*>(g);
        Ebb<Manager> mgr = static_cast<Ebb<Manager> >
          (static_cast<LocalEntry*>(ge->mgr_)); //requires destructor
        bool ret = mgr->preCall(args, fnum, fret);
        if (ret) {
          event::altstackPush(reinterpret_cast<uintptr_t>
                       (static_cast<LocalEntry*>(mgr)));
        }
        return ret;
      }

      extern "C" uintptr_t postcall(uintptr_t ret)
      {
        Ebb<Manager> mgr = static_cast<Ebb<Manager> >
          (reinterpret_cast<LocalEntry*>(event::altstackPop())); //destructor
        return mgr->postCall(ret);
      }

      extern "C" void defaultFunc0();
      extern "C" void defaultFunc1();
      extern "C" void defaultFunc2();
      extern "C" void defaultFunc3();
      extern "C" void defaultFunc4();
      extern "C" void defaultFunc5();
      extern "C" void defaultFunc6();
      extern "C" void defaultFunc7();
      extern "C" void defaultFunc8();
      extern "C" void defaultFunc9();

      extern "C" void defaultFunc10();
      extern "C" void defaultFunc11();
      extern "C" void defaultFunc12();
      extern "C" void defaultFunc13();
      extern "C" void defaultFunc14();
      extern "C" void defaultFunc15();
      extern "C" void defaultFunc16();
      extern "C" void defaultFunc17();
      extern "C" void defaultFunc18();
      extern "C" void defaultFunc19();

      extern "C" void defaultFunc20();
      extern "C" void defaultFunc21();
      extern "C" void defaultFunc22();
      extern "C" void defaultFunc23();
      extern "C" void defaultFunc24();
      extern "C" void defaultFunc25();
      extern "C" void defaultFunc26();
      extern "C" void defaultFunc27();
      extern "C" void defaultFunc28();
      extern "C" void defaultFunc29();

      extern "C" void defaultFunc30();
      extern "C" void defaultFunc31();
      extern "C" void defaultFunc32();
      extern "C" void defaultFunc33();
      extern "C" void defaultFunc34();
      extern "C" void defaultFunc35();
      extern "C" void defaultFunc36();
      extern "C" void defaultFunc37();
      extern "C" void defaultFunc38();
      extern "C" void defaultFunc39();

      extern "C" void defaultFunc40();
      extern "C" void defaultFunc41();
      extern "C" void defaultFunc42();
      extern "C" void defaultFunc43();
      extern "C" void defaultFunc44();
      extern "C" void defaultFunc45();
      extern "C" void defaultFunc46();
      extern "C" void defaultFunc47();
      extern "C" void defaultFunc48();
      extern "C" void defaultFunc49();

      extern "C" void defaultFunc50();
      extern "C" void defaultFunc51();
      extern "C" void defaultFunc52();
      extern "C" void defaultFunc53();
      extern "C" void defaultFunc54();
      extern "C" void defaultFunc55();
      extern "C" void defaultFunc56();
      extern "C" void defaultFunc57();
      extern "C" void defaultFunc58();
      extern "C" void defaultFunc59();

      extern "C" void defaultFunc60();
      extern "C" void defaultFunc61();
      extern "C" void defaultFunc62();
      extern "C" void defaultFunc63();
      extern "C" void defaultFunc64();
      extern "C" void defaultFunc65();
      extern "C" void defaultFunc66();
      extern "C" void defaultFunc67();
      extern "C" void defaultFunc68();
      extern "C" void defaultFunc69();

      extern "C" void defaultFunc70();
      extern "C" void defaultFunc71();
      extern "C" void defaultFunc72();
      extern "C" void defaultFunc73();
      extern "C" void defaultFunc74();
      extern "C" void defaultFunc75();
      extern "C" void defaultFunc76();
      extern "C" void defaultFunc77();
      extern "C" void defaultFunc78();
      extern "C" void defaultFunc79();

      extern "C" void defaultFunc80();
      extern "C" void defaultFunc81();
      extern "C" void defaultFunc82();
      extern "C" void defaultFunc83();
      extern "C" void defaultFunc84();
      extern "C" void defaultFunc85();
      extern "C" void defaultFunc86();
      extern "C" void defaultFunc87();
      extern "C" void defaultFunc88();
      extern "C" void defaultFunc89();

      extern "C" void defaultFunc90();
      extern "C" void defaultFunc91();
      extern "C" void defaultFunc92();
      extern "C" void defaultFunc93();
      extern "C" void defaultFunc94();
      extern "C" void defaultFunc95();
      extern "C" void defaultFunc96();
      extern "C" void defaultFunc97();
      extern "C" void defaultFunc98();
      extern "C" void defaultFunc99();

      extern "C" void defaultFunc100();
      extern "C" void defaultFunc101();
      extern "C" void defaultFunc102();
      extern "C" void defaultFunc103();
      extern "C" void defaultFunc104();
      extern "C" void defaultFunc105();
      extern "C" void defaultFunc106();
      extern "C" void defaultFunc107();
      extern "C" void defaultFunc108();
      extern "C" void defaultFunc109();

      extern "C" void defaultFunc110();
      extern "C" void defaultFunc111();
      extern "C" void defaultFunc112();
      extern "C" void defaultFunc113();
      extern "C" void defaultFunc114();
      extern "C" void defaultFunc115();
      extern "C" void defaultFunc116();
      extern "C" void defaultFunc117();
      extern "C" void defaultFunc118();
      extern "C" void defaultFunc119();

      extern "C" void defaultFunc120();
      extern "C" void defaultFunc121();
      extern "C" void defaultFunc122();
      extern "C" void defaultFunc123();
      extern "C" void defaultFunc124();
      extern "C" void defaultFunc125();
      extern "C" void defaultFunc126();
      extern "C" void defaultFunc127();
      extern "C" void defaultFunc128();
      extern "C" void defaultFunc129();

      extern "C" void defaultFunc130();
      extern "C" void defaultFunc131();
      extern "C" void defaultFunc132();
      extern "C" void defaultFunc133();
      extern "C" void defaultFunc134();
      extern "C" void defaultFunc135();
      extern "C" void defaultFunc136();
      extern "C" void defaultFunc137();
      extern "C" void defaultFunc138();
      extern "C" void defaultFunc139();

      extern "C" void defaultFunc140();
      extern "C" void defaultFunc141();
      extern "C" void defaultFunc142();
      extern "C" void defaultFunc143();
      extern "C" void defaultFunc144();
      extern "C" void defaultFunc145();
      extern "C" void defaultFunc146();
      extern "C" void defaultFunc147();
      extern "C" void defaultFunc148();
      extern "C" void defaultFunc149();

      extern "C" void defaultFunc150();
      extern "C" void defaultFunc151();
      extern "C" void defaultFunc152();
      extern "C" void defaultFunc153();
      extern "C" void defaultFunc154();
      extern "C" void defaultFunc155();
      extern "C" void defaultFunc156();
      extern "C" void defaultFunc157();
      extern "C" void defaultFunc158();
      extern "C" void defaultFunc159();

      extern "C" void defaultFunc160();
      extern "C" void defaultFunc161();
      extern "C" void defaultFunc162();
      extern "C" void defaultFunc163();
      extern "C" void defaultFunc164();
      extern "C" void defaultFunc165();
      extern "C" void defaultFunc166();
      extern "C" void defaultFunc167();
      extern "C" void defaultFunc168();
      extern "C" void defaultFunc169();

      extern "C" void defaultFunc170();
      extern "C" void defaultFunc171();
      extern "C" void defaultFunc172();
      extern "C" void defaultFunc173();
      extern "C" void defaultFunc174();
      extern "C" void defaultFunc175();
      extern "C" void defaultFunc176();
      extern "C" void defaultFunc177();
      extern "C" void defaultFunc178();
      extern "C" void defaultFunc179();

      extern "C" void defaultFunc180();
      extern "C" void defaultFunc181();
      extern "C" void defaultFunc182();
      extern "C" void defaultFunc183();
      extern "C" void defaultFunc184();
      extern "C" void defaultFunc185();
      extern "C" void defaultFunc186();
      extern "C" void defaultFunc187();
      extern "C" void defaultFunc188();
      extern "C" void defaultFunc189();

      extern "C" void defaultFunc190();
      extern "C" void defaultFunc191();
      extern "C" void defaultFunc192();
      extern "C" void defaultFunc193();
      extern "C" void defaultFunc194();
      extern "C" void defaultFunc195();
      extern "C" void defaultFunc196();
      extern "C" void defaultFunc197();
      extern "C" void defaultFunc198();
      extern "C" void defaultFunc199();

      extern "C" void defaultFunc200();
      extern "C" void defaultFunc201();
      extern "C" void defaultFunc202();
      extern "C" void defaultFunc203();
      extern "C" void defaultFunc204();
      extern "C" void defaultFunc205();
      extern "C" void defaultFunc206();
      extern "C" void defaultFunc207();
      extern "C" void defaultFunc208();
      extern "C" void defaultFunc209();

      extern "C" void defaultFunc210();
      extern "C" void defaultFunc211();
      extern "C" void defaultFunc212();
      extern "C" void defaultFunc213();
      extern "C" void defaultFunc214();
      extern "C" void defaultFunc215();
      extern "C" void defaultFunc216();
      extern "C" void defaultFunc217();
      extern "C" void defaultFunc218();
      extern "C" void defaultFunc219();

      extern "C" void defaultFunc220();
      extern "C" void defaultFunc221();
      extern "C" void defaultFunc222();
      extern "C" void defaultFunc223();
      extern "C" void defaultFunc224();
      extern "C" void defaultFunc225();
      extern "C" void defaultFunc226();
      extern "C" void defaultFunc227();
      extern "C" void defaultFunc228();
      extern "C" void defaultFunc229();

      extern "C" void defaultFunc230();
      extern "C" void defaultFunc231();
      extern "C" void defaultFunc232();
      extern "C" void defaultFunc233();
      extern "C" void defaultFunc234();
      extern "C" void defaultFunc235();
      extern "C" void defaultFunc236();
      extern "C" void defaultFunc237();
      extern "C" void defaultFunc238();
      extern "C" void defaultFunc239();

      extern "C" void defaultFunc240();
      extern "C" void defaultFunc241();
      extern "C" void defaultFunc242();
      extern "C" void defaultFunc243();
      extern "C" void defaultFunc244();
      extern "C" void defaultFunc245();
      extern "C" void defaultFunc246();
      extern "C" void defaultFunc247();
      extern "C" void defaultFunc248();
      extern "C" void defaultFunc249();

      extern "C" void defaultFunc250();
      extern "C" void defaultFunc251();
      extern "C" void defaultFunc252();
      extern "C" void defaultFunc253();
      extern "C" void defaultFunc254();
      extern "C" void defaultFunc255();

      void (*defaultVtable[256])() = {
        defaultFunc0,
        defaultFunc1,
        defaultFunc2,
        defaultFunc3,
        defaultFunc4,
        defaultFunc5,
        defaultFunc6,
        defaultFunc7,
        defaultFunc8,
        defaultFunc9,
        defaultFunc10,
        defaultFunc11,
        defaultFunc12,
        defaultFunc13,
        defaultFunc14,
        defaultFunc15,
        defaultFunc16,
        defaultFunc17,
        defaultFunc18,
        defaultFunc19,
        defaultFunc20,
        defaultFunc21,
        defaultFunc22,
        defaultFunc23,
        defaultFunc24,
        defaultFunc25,
        defaultFunc26,
        defaultFunc27,
        defaultFunc28,
        defaultFunc29,
        defaultFunc30,
        defaultFunc31,
        defaultFunc32,
        defaultFunc33,
        defaultFunc34,
        defaultFunc35,
        defaultFunc36,
        defaultFunc37,
        defaultFunc38,
        defaultFunc39,
        defaultFunc40,
        defaultFunc41,
        defaultFunc42,
        defaultFunc43,
        defaultFunc44,
        defaultFunc45,
        defaultFunc46,
        defaultFunc47,
        defaultFunc48,
        defaultFunc49,
        defaultFunc50,
        defaultFunc51,
        defaultFunc52,
        defaultFunc53,
        defaultFunc54,
        defaultFunc55,
        defaultFunc56,
        defaultFunc57,
        defaultFunc58,
        defaultFunc59,
        defaultFunc60,
        defaultFunc61,
        defaultFunc62,
        defaultFunc63,
        defaultFunc64,
        defaultFunc65,
        defaultFunc66,
        defaultFunc67,
        defaultFunc68,
        defaultFunc69,
        defaultFunc70,
        defaultFunc71,
        defaultFunc72,
        defaultFunc73,
        defaultFunc74,
        defaultFunc75,
        defaultFunc76,
        defaultFunc77,
        defaultFunc78,
        defaultFunc79,
        defaultFunc80,
        defaultFunc81,
        defaultFunc82,
        defaultFunc83,
        defaultFunc84,
        defaultFunc85,
        defaultFunc86,
        defaultFunc87,
        defaultFunc88,
        defaultFunc89,
        defaultFunc90,
        defaultFunc91,
        defaultFunc92,
        defaultFunc93,
        defaultFunc94,
        defaultFunc95,
        defaultFunc96,
        defaultFunc97,
        defaultFunc98,
        defaultFunc99,
        defaultFunc100,
        defaultFunc101,
        defaultFunc102,
        defaultFunc103,
        defaultFunc104,
        defaultFunc105,
        defaultFunc106,
        defaultFunc107,
        defaultFunc108,
        defaultFunc109,
        defaultFunc110,
        defaultFunc111,
        defaultFunc112,
        defaultFunc113,
        defaultFunc114,
        defaultFunc115,
        defaultFunc116,
        defaultFunc117,
        defaultFunc118,
        defaultFunc119,
        defaultFunc120,
        defaultFunc121,
        defaultFunc122,
        defaultFunc123,
        defaultFunc124,
        defaultFunc125,
        defaultFunc126,
        defaultFunc127,
        defaultFunc128,
        defaultFunc129,
        defaultFunc130,
        defaultFunc131,
        defaultFunc132,
        defaultFunc133,
        defaultFunc134,
        defaultFunc135,
        defaultFunc136,
        defaultFunc137,
        defaultFunc138,
        defaultFunc139,
        defaultFunc140,
        defaultFunc141,
        defaultFunc142,
        defaultFunc143,
        defaultFunc144,
        defaultFunc145,
        defaultFunc146,
        defaultFunc147,
        defaultFunc148,
        defaultFunc149,
        defaultFunc150,
        defaultFunc151,
        defaultFunc152,
        defaultFunc153,
        defaultFunc154,
        defaultFunc155,
        defaultFunc156,
        defaultFunc157,
        defaultFunc158,
        defaultFunc159,
        defaultFunc160,
        defaultFunc161,
        defaultFunc162,
        defaultFunc163,
        defaultFunc164,
        defaultFunc165,
        defaultFunc166,
        defaultFunc167,
        defaultFunc168,
        defaultFunc169,
        defaultFunc170,
        defaultFunc171,
        defaultFunc172,
        defaultFunc173,
        defaultFunc174,
        defaultFunc175,
        defaultFunc176,
        defaultFunc177,
        defaultFunc178,
        defaultFunc179,
        defaultFunc180,
        defaultFunc181,
        defaultFunc182,
        defaultFunc183,
        defaultFunc184,
        defaultFunc185,
        defaultFunc186,
        defaultFunc187,
        defaultFunc188,
        defaultFunc189,
        defaultFunc190,
        defaultFunc191,
        defaultFunc192,
        defaultFunc193,
        defaultFunc194,
        defaultFunc195,
        defaultFunc196,
        defaultFunc197,
        defaultFunc198,
        defaultFunc199,
        defaultFunc200,
        defaultFunc201,
        defaultFunc202,
        defaultFunc203,
        defaultFunc204,
        defaultFunc205,
        defaultFunc206,
        defaultFunc207,
        defaultFunc208,
        defaultFunc209,
        defaultFunc210,
        defaultFunc211,
        defaultFunc212,
        defaultFunc213,
        defaultFunc214,
        defaultFunc215,
        defaultFunc216,
        defaultFunc217,
        defaultFunc218,
        defaultFunc219,
        defaultFunc220,
        defaultFunc221,
        defaultFunc222,
        defaultFunc223,
        defaultFunc224,
        defaultFunc225,
        defaultFunc226,
        defaultFunc227,
        defaultFunc228,
        defaultFunc229,
        defaultFunc230,
        defaultFunc231,
        defaultFunc232,
        defaultFunc233,
        defaultFunc234,
        defaultFunc235,
        defaultFunc236,
        defaultFunc237,
        defaultFunc238,
        defaultFunc239,
        defaultFunc240,
        defaultFunc241,
        defaultFunc242,
        defaultFunc243,
        defaultFunc244,
        defaultFunc245,
        defaultFunc246,
        defaultFunc247,
        defaultFunc248,
        defaultFunc249,
        defaultFunc250,
        defaultFunc251,
        defaultFunc252,
        defaultFunc253,
        defaultFunc254,
        defaultFunc255
      };
    }
  }
}
