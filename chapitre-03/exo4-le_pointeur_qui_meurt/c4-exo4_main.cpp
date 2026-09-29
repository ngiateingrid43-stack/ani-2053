#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <cstdio>
#include <cstring>
#include <utility>   

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "c4-exo4";
    d.appVersion = "1.0.0";
    return d;
})());

int nkmain(const NkEntryState& state) {
    (void)state;
    NkWindowConfig cfg;
    cfg.title = "c4-exo4 - mode BUG (C = corrige)"; cfg.width = 700; cfg.height = 300; cfg.centered = true;
    NkWindow window;
    if (!window.Create(cfg)) return -1;

    bool corrige = false;
    NkEvent*   garde = nullptr;       
    NkEventPtr copie;                 
    char       instantane[512] = "";  

    while (window.IsOpen()) {
        int dansLaTrame = 0;
        if (!corrige) {
            while (NkEvent* ev = NkEvents().PollEvent()) {
                ++dansLaTrame;
                if (ev->Is<NkWindowCloseEvent>()) window.Close();
                if (auto* kp = ev->As<NkKeyPressEvent>()) if (kp->GetKey() == NkKey::NK_C) {
                    corrige = true; garde = nullptr; window.SetTitle("c4-exo4 - mode CORRIGE (PollEventCopy)"); std::printf("--- mode CORRIGE ---\n");
                    continue;
                }
                garde = ev;                                                        
                std::snprintf(instantane, sizeof(instantane), "%s", ev->ToString().CStr());
            }
            if (garde) {                                                           
                const NkString relu = garde->ToString();                           
                const bool pareil = std::strcmp(relu.CStr(), instantane) == 0;
                std::printf("trame : %d evt(s) | garde->\"%s\" | %s\n", dansLaTrame, relu.CStr(), pareil ? "identique" : "!!! CONTENU CHANGE / pointeur perime");
            }
        } else {
            while (NkEventPtr e = NkEvents().PollEventCopy()) {
                ++dansLaTrame;
                if (e->Is<NkWindowCloseEvent>()) window.Close();
                copie = std::move(e);                                              
                std::snprintf(instantane, sizeof(instantane), "%s", copie->ToString().CStr());
            }
            if (copie) {
                const NkString relu = copie->ToString();
                const bool pareil = std::strcmp(relu.CStr(), instantane) == 0;
                std::printf("trame : %d evt(s) | copie->\"%s\" | %s\n", dansLaTrame, relu.CStr(), pareil ? "identique (sur)" : "DIFFERENT");
            }
        }
        NkChrono::Sleep((int64)16);
    }
    return 0;
}