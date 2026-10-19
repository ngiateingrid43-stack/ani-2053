// Exercice 5 - Le futur qui ne bloque pas
// Usage : futur [get|isready] [grande-photo.png]
// Ecrit cadence-get.txt ou cadence-isready.txt.
//
// Contournement temporaire : std::promise/std::future sont utilises ici,
// car la version actuelle de NkFuture.h semble incompatible avec les autres
// en-tetes de NkentseuKit (MakeSharedPtr et WaitUntil).

#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindowConfig.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKTime/NkTime.h"
#include "NKTime/NkChrono.h"
#include "NKLogger/NkLog.h"
#include "NKMath/NkColor.h"
#include "NKCanvas/Core/NkContextDesc.h"
#include "NKCanvas/Renderer/Targets/NkRenderWindow.h"
#include "NKCanvas/Renderer/Core/NkRenderer2D.h"
#include "NKImage/Core/NkImage.h"
#include "NKThreading/NkThreadPool.h"
#include "NKCore/NkTraits.h"

#include <cstdio>
#include <cstring>
#include <future>
#include <chrono>

using namespace nkentseu;
using namespace nkentseu::threading;
using namespace nkentseu::renderer;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Futur";
    d.appVersion = "1.0.0";
    return d;
})());

// true : mode Get (attente bloquante)
// false : mode IsReady (verification sans attente)
static const bool kModeGet = true;

static long long TailleFichier(const char* chemin) {
    FILE* f = std::fopen(chemin, "rb");
    if (!f) return -1;

    std::fseek(f, 0, SEEK_END);
    const long long taille = static_cast<long long>(std::ftell(f));
    std::fclose(f);
    return taille;
}

int nkmain(const NkEntryState& state) {
    bool modeGet = kModeGet;
    const char* chemin = "C:/Users/Stark/Downloads/WhatsApp Image 2026-10-06 at 00.45.44.jpeg";

    for (usize i = 1; i < state.args.Size(); ++i) {
        const char* arg = state.args[i].CStr();

        if (std::strcmp(arg, "get") == 0) {
            modeGet = true;
        } else if (std::strcmp(arg, "isready") == 0) {
            modeGet = false;
        } else {
            chemin = arg;
        }
    }

    NkWindowConfig cfg;
    cfg.title = "Futur";
    cfg.width = 900;
    cfg.height = 400;
    cfg.centered = true;
    cfg.resizable = false;

    NkWindow window;
    if (!window.Create(cfg)) {
        logger.Error("[futur] fenetre impossible");
        return -1;
    }

    NkContextDesc desc;
    desc.api = NkGraphicsApi::NK_GFX_API_DX11;
    NkRenderWindow target(window, desc);

    if (!target.IsValid()) {
        window.Close();
        return -2;
    }

    bool running = true;
    auto& events = NkEvents();
    events.AddEventCallback<NkWindowCloseEvent>(
        [&](NkWindowCloseEvent*) { running = false; });

    // std::promise/std::future remplacent temporairement NkPromise/NkFuture.
    // La promesse vit jusqu'a la fin de la tache du pool.
    std::promise<NkImage> promesse;
    std::future<NkImage> futur = promesse.get_future();
    NkThreadPool pool;

    NkImage imageStock;
    bool imageChargee = false;
    bool lance = false;
    bool pret = false;

    float64 tempsChargementMs = 0.0;
    int32 trame = 0;
    int32 trameDansChargement = 0;
    int32 tramesApresPret = 0;
    float64 trameMax = 0.0;
    float32 x = 0.f;

    while (running && window.IsOpen()) {
        NkChrono chronoTrame;

        while (NkEvent* ev = events.PollEvent()) {
            (void)ev;
        }
        if (!running) break;

        bool dansFenetre = false;

        if (!lance && trame == 60) {
            lance = true;
            dansFenetre = true;

            pool.Enqueue([&promesse, &tempsChargementMs, chemin]() {
                NkChrono chronoChargement;
                NkImage img;

                const bool succes = img.Load(chemin);
                tempsChargementMs = chronoChargement.Elapsed().milliseconds;

                if (!succes) {
                    logger.Errorf("[futur] chargement impossible : %s", chemin);
                }

                promesse.set_value(nkentseu::traits::NkMove(img));
            });

            if (!modeGet) {
                ++trameDansChargement;
            }

            if (modeGet) {
                // Bloque ici jusqu'a la fin du chargement.
                imageStock = futur.get();
                imageChargee = true;
                pret = true;
            }
        } else if (lance && !pret) {
            dansFenetre = true;

            if (modeGet) {
                // Normalement deja pret apres le get() de la trame 60.
                if (futur.valid() &&
                    futur.wait_for(std::chrono::seconds(0)) ==
                        std::future_status::ready) {
                    imageStock = futur.get();
                    imageChargee = true;
                    pret = true;
                }
            } else {
                // Verification non bloquante : la boucle continue a dessiner.
                if (futur.wait_for(std::chrono::seconds(0)) ==
                    std::future_status::ready) {
                    imageStock = futur.get();
                    imageChargee = true;
                    pret = true;
                } else {
                    ++trameDansChargement;
                }
            }
        }

        const math::NkVec2u taille = target.GetSize();
        const float32 largeurFenetre = static_cast<float32>(taille.x);

        target.Clear(NkColor2D{18, 20, 28, 255});

        NkRenderer2D& renderer = target.GetRenderer2D();
        renderer.DrawFilledRect(
            NkRect2f{x, 180.f, 40.f, 40.f},
            pret ? NkColor2D{90, 220, 120, 255}
                 : NkColor2D{255, 180, 60, 255});

        target.Display();

        x += 1.f;
        if (x > largeurFenetre) x = 0.f;

        const float64 dureeTrameMs = chronoTrame.Elapsed().milliseconds;
        if (dansFenetre && dureeTrameMs > trameMax) {
            trameMax = dureeTrameMs;
        }

        ++trame;
        if (pret && ++tramesApresPret > 60) {
            running = false;
        }
    }

    // Attend la fin de la tache avant de detruire les objets captures.
    pool.Join();

    // Si la fenetre a ete fermee avant la recuperation du resultat,
    // recupere tout de meme l'image apres la fin de la tache.
    if (lance && !imageChargee &&
        futur.valid() &&
        futur.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        imageStock = futur.get();
        imageChargee = true;
    }

    const long long tailleFichier = TailleFichier(chemin);
    const int32 largeur = imageChargee ? imageStock.Width() : 0;
    const int32 hauteur = imageChargee ? imageStock.Height() : 0;
    const int32 octetsParPixel = imageChargee ? imageStock.BytesPP() : 0;

    const long long octetsDecode = static_cast<long long>(largeur) *
                                   static_cast<long long>(hauteur) *
                                   static_cast<long long>(octetsParPixel);

    const char* mode = modeGet ? "Get" : "IsReady";

    logger.Infof(
        "[futur] image %dx%d, %lld octets decodes (fichier %lld), chargement %.3f ms",
        static_cast<int>(largeur), static_cast<int>(hauteur),
        octetsDecode, tailleFichier, tempsChargementMs);

    logger.Infof(
        "[futur] %s : trame la plus longue %.3f ms, %d trame(s) dessinee(s) pendant le chargement",
        mode, trameMax, static_cast<int>(trameDansChargement));

    FILE* out = std::fopen(
        modeGet ? "cadence-get.txt" : "cadence-isready.txt", "w");

    if (out) {
        std::fprintf(
            out,
            "Image : %dx%d pixels, %lld octets decodes (fichier de %lld octets), chargement en %.3f ms\n",
            static_cast<int>(largeur), static_cast<int>(hauteur),
            octetsDecode, tailleFichier, tempsChargementMs);

        std::fprintf(
            out,
            "%s : trame la plus longue %.3f ms, %d trames pendant le chargement\n",
            mode, trameMax, static_cast<int>(trameDansChargement));

        std::fclose(out);
    }

    return 0;
}
