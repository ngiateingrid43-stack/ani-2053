#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkChrono.h"
#include "NKThreading/NkThread.h"
#include "NKThreading/NkMutex.h"
#include "NKThreading/NkScopedLock.h"

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Etreinte";
    d.appVersion = "1.0.0";
    return d;
})());

static NkMutex m1; // ordre : m1 d'abord
static NkMutex m2; // puis m2

// ====================== VERSION QUI SE FIGE (en commentaire) ======================
// int nkmain(const NkEntryState &) {
//     NkThread a([]() {
//         NkScopedLock<NkMutex> v1(m1);                       // a prend m1
//         logger.Info("[a] m1 pris");
//         NkChrono::SleepMilliseconds(static_cast<int64>(100));
//         NkScopedLock<NkMutex> v2(m2);                       // a attend m2 (tenu par b)
//         logger.Info("[a] m2 pris");
//     });
//     NkThread b([]() {
//         NkScopedLock<NkMutex> v2(m2);                       // b prend m2
//         logger.Info("[b] m2 pris");
//         NkChrono::SleepMilliseconds(static_cast<int64>(100));
//         NkScopedLock<NkMutex> v1(m1);                       // b attend m1 (tenu par a)
//         logger.Info("[b] m1 pris");
//     });
//     a.Join();
//     b.Join();
//     return 0;
// }
// ==================================================================================

// ====================== VERSION CORRIGEE (active) ======================
int nkmain(const NkEntryState &) {
    NkThread a([]() {
        NkScopedLock<NkMutex> v1(m1); // m1 d'abord
        logger.Info("[a] m1 pris");
        NkChrono::SleepMilliseconds(static_cast<int64>(100));
        NkScopedLock<NkMutex> v2(m2); // puis m2
        logger.Info("[a] m2 pris");
    });
    NkThread b([]() {
        NkScopedLock<NkMutex> v1(m1); // m1 d'abord, comme a : meme ordre partout
        logger.Info("[b] m1 pris");
        NkChrono::SleepMilliseconds(static_cast<int64>(100));
        NkScopedLock<NkMutex> v2(m2); // puis m2
        logger.Info("[b] m2 pris");
    });
    a.SetName("fil-a");
    b.SetName("fil-b");
    a.Join();
    b.Join();
    logger.Info("[etreinte] termine sans blocage");
    return 0;
}
