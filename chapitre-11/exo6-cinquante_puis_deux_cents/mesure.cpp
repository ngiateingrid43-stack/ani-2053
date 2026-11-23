// Mesure de l'enregistrement et de la relecture d'une planche NkRef, avec le
// vrai format de l'outil (NkRefFile) et son chronometre (NkChrono).
//
// Construction, depuis la racine du depot du moteur :
//   g++ -std=c++17 -O0 -g -IApplications/NkRef/src \
//       -IKernel/Foundation/NKMath/src -IKernel/Foundation/NKContainers/src \
//       -IKernel/Foundation/NKCore/src -IKernel/Foundation/NKMemory/src \
//       -IKernel/Foundation/NKPlatform/src -IKernel/System/NKTime/src \
//       mesure.cpp <objets du moteur> -o mesure
// Lancement : ./mesure 50

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "NkRef/NkRefFile.h"
#include "NKTime/NkChrono.h" // apres NkRefFile.h : NKMath exige <ctime> deja inclus

namespace {

const int kLargeur = 100;
const int kHauteur = 80;
const int kOctetsSource = 1000;
const int kRepetitions = 20;

struct NkPlanche {
        nkref::NkRefBoard board;
        nkref::NkRefView view;
        nkentseu::NkVector<nkref::NkRefFileImage> images;
        nkentseu::NkVector<nkentseu::NkVector<nkentseu::uint8>> sources;
};

void Construire(NkPlanche &planche, int nombre) {
        planche.sources.Resize(static_cast<nkentseu::usize>(nombre));
        planche.images.Resize(static_cast<nkentseu::usize>(nombre));
        for (int i = 0; i < nombre; ++i) {
                nkentseu::NkVector<nkentseu::uint8> &octets = planche.sources[static_cast<nkentseu::usize>(i)];
                octets.Resize(static_cast<nkentseu::usize>(kOctetsSource));
                for (int k = 0; k < kOctetsSource; ++k)
                        octets[static_cast<nkentseu::usize>(k)] =
                                static_cast<nkentseu::uint8>((i * 31 + k * 7) & 0xFF);

                // Les images sont posees en grille, sans aucun trait de crayon :
                // l'espacement laisse la place a la taille affichee.
                const nkentseu::math::NkVec2f position{
                        static_cast<nkentseu::float32>((i % 10) * (kLargeur + 20) - 600),
                        static_cast<nkentseu::float32>((i / 10) * (kHauteur + 20) - 400)};
                planche.board.AddItem(position, kLargeur, kHauteur, nkentseu::NkString());

                planche.images[static_cast<nkentseu::usize>(i)].data = octets.Data();
                planche.images[static_cast<nkentseu::usize>(i)].size = octets.Size();
        }
}

} // namespace

int main(int argc, char **argv) {
        const int nombre = argc > 1 ? std::atoi(argv[1]) : 50;

        NkPlanche planche;
        Construire(planche, nombre);

        nkentseu::NkVector<nkentseu::uint8> ecrit;
        nkentseu::NkChrono chrono;
        double totalEcriture = 0.0, totalLecture = 0.0;

        for (int i = 0; i < kRepetitions; ++i) {
                nkentseu::NkElapsedTime debutEcriture = chrono.Reset();
                nkref::NkRefSerialize(ecrit, planche.board, planche.images.Data(), planche.view, true, true);
                totalEcriture += chrono.Elapsed().ToMilliseconds();
                (void)debutEcriture;
        }

        nkentseu::NkVector<nkref::NkRefLoadedItem> relus;
        nkentseu::NkVector<nkref::NkRefStroke> traits;
        bool theme = true, grille = true;
        bool lisible = true;

        for (int i = 0; i < kRepetitions; ++i) {
                nkref::NkRefView vue;
                nkentseu::NkChrono lecture;
                nkref::NkRefDeserialize(ecrit.Data(), ecrit.Size(), relus, traits, vue, theme, grille);
                totalLecture += lecture.Elapsed().ToMilliseconds();
                if (relus.Size() != static_cast<nkentseu::usize>(nombre))
                        lisible = false;
        }

        std::printf("images : %d, octets : %u, ecrire : %.3f ms, relire : %.3f ms\n", nombre,
                    static_cast<unsigned>(ecrit.Size()), totalEcriture / kRepetitions,
                    totalLecture / kRepetitions);
        if (!lisible)
                std::printf("la relecture n'a pas rendu les %d images\n", nombre);
        return 0;
}
