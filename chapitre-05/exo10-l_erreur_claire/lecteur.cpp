// Exercice 10 - L'erreur claire
// Ouvre trois fichiers avec le lecteur video de NKMedia (Open), les sonde (ProbeFile),
// compte les images lues (ReadFrame) et ecrit resultats.txt.
#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKMedia/Video/NkVideoReader.h"
#include "NKMedia/NkMediaProbe.h"

#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::media;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Lecteur";
    d.appVersion = "1.0.0";
    return d;
})());

static const int kNbFichiers = 3;

static const char *NomSansDossier(const char *chemin) {
    const char *nom = chemin;
    for (const char *p = chemin; *p; ++p)
        if (*p == '/' || *p == '\\') nom = p + 1;
    return nom;
}

static const char *NomTypePiste(NkMediaTrackType t) {
    if (t == NkMediaTrackType::NK_VIDEO) return "video";
    if (t == NkMediaTrackType::NK_AUDIO) return "audio";
    return "inconnu";
}

int nkmain(const NkEntryState &state) {
    if (static_cast<int>(state.args.Size()) < kNbFichiers + 1) {
        logger.Error("[lecteur] usage : lecteur fichier1 fichier2 fichier3");
        return -1;
    }

    FILE *out = std::fopen("resultats.txt", "w");
    if (!out) {
        logger.Error("[lecteur] impossible d'ecrire resultats.txt");
        return -2;
    }

    for (int i = 0; i < kNbFichiers; ++i) {
        const char *chemin = state.args[static_cast<usize>(i + 1)].CStr();
        const char *nom = NomSansDossier(chemin);

        // --- Le lecteur : Open ne rend que vrai ou faux. On teste son retour AVANT de lire.
        NkVideoReader lecteur;
        bool ouvert = lecteur.Open(chemin);
        int images = 0;
        if (ouvert) {
            const NkVideoReaderInfo &info = lecteur.Info();
            logger.Infof("[lecteur] %s ouvert : conteneur=%s codec=%s %dx%d", nom, info.container.CStr(),
                         info.codec.CStr(), static_cast<int>(info.width), static_cast<int>(info.height));
            NkVideoFrame trame; // declaree hors de la boucle : la capacite est reutilisee
            while (lecteur.ReadFrame(trame)) ++images;
            lecteur.Close();
            logger.Infof("[lecteur] %s : %d image(s) lue(s)", nom, images);
        } else {
            logger.Errorf("[lecteur] %s : Open a refuse le fichier (la cause n'est pas dite : voir la sonde)", nom);
        }

        // --- La sonde : instantanee, ne decode aucune image ; c'est elle qui aide a trouver la cause.
        NkMediaInfo sonde;
        const bool sondeOk = NkMediaProbe::ProbeFile(chemin, sonde);
        const char *conteneur = "ECHEC";
        if (sondeOk) {
            conteneur = sonde.ContainerName();
            logger.Infof("[lecteur] %s sonde : conteneur=%s, %d piste(s)", nom, conteneur,
                         static_cast<int>(sonde.tracks.Size()));
            for (usize k = 0; k < sonde.tracks.Size(); ++k) {
                const NkMediaTrack &t = sonde.tracks[k];
                logger.Infof("[lecteur]   piste %d : %s codec=%s", static_cast<int>(k + 1), NomTypePiste(t.type),
                             t.codec.CStr());
            }
        } else {
            logger.Errorf("[lecteur] %s : la sonde a echoue aussi", nom);
        }

        std::fprintf(out, "fichier : %s, ouvert : %s, images : %d, sonde : %s\n", nom, ouvert ? "oui" : "non",
                     images, conteneur);
    }
    std::fclose(out);
    logger.Info("[lecteur] resultats.txt ecrit");
    return 0;
}
