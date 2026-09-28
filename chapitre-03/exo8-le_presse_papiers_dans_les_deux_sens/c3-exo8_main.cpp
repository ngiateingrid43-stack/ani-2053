#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "Chapitre03-Exo8";
    d.appVersion = "0.1.0";
    return d;
})());

static void RoundTripText(NkWindow& window) {
    NkString text = window.GetClipboardText();
    if (text.Empty()) {
        std::printf("[texte] presse-papiers vide ou pas de texte disponible.\n");
        return;
    }
    std::printf("[texte] avant : \"%s\"\n", text.CStr());
    text.ToUpper();
    window.SetClipboardText(text);
    std::printf("[texte] apres : \"%s\" (remis dans le presse-papiers)\n", text.CStr());
}

static void RoundTripImage(NkWindow& window) {
    if (!window.HasClipboardImage()) {
        std::printf("[image] pas d'image dans le presse-papiers (copie une capture d'ecran, puis reessaie).\n");
        return;
    }

    NkClipboardImage img;
    if (!window.GetClipboardImage(img) || !img.IsValid()) {
        std::printf("[image] echec de lecture du presse-papiers.\n");
        return;
    }
    std::printf("[image] lue : %ux%u, %zu octets RGBA\n", img.width, img.height, (usize)img.pixels.Size());

    // Inversion des couleurs : on garde l'alpha, on inverse R, G, B.
    const usize pixelCount = (usize)img.width * img.height;
    for (usize i = 0; i < pixelCount; ++i) {
        uint8* px = &img.pixels[i * 4];
        px[0] = 255 - px[0];   // R
        px[1] = 255 - px[1];   // G
        px[2] = 255 - px[2];   // B
        // px[3] (A) inchange
    }

    if (window.SetClipboardImage(img)) {
        std::printf("[image] couleurs inversees et remises dans le presse-papiers.\n");
    }
    else {
        std::printf("[image] echec d'ecriture dans le presse-papiers.\n");
    }
}

int nkmain(const NkEntryState& state) {
    NkWindowConfig cfg;
    cfg.title    = "Exo8 - T=texte, I=image";
    cfg.width    = 800;
    cfg.height   = 450;
    cfg.centered = true;

    NkWindow window;
    if (!window.Create(cfg)) {
        return -1;
    }

    std::printf("Copie un texte puis appuie sur T. Copie une image (capture d'ecran) puis appuie sur I.\n");

    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();
            }
            else if (auto* kp = ev->As<NkKeyPressEvent>()) {
                if (kp->GetKey() == NkKey::NK_T) {
                    RoundTripText(window);
                }
                else if (kp->GetKey() == NkKey::NK_I) {
                    RoundTripImage(window);
                }
            }
        }

        NkChrono::Sleep((int64)8);
    }

    return 0;
}
