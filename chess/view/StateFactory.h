#ifndef CHESS_STATEFACTORY_H
#define CHESS_STATEFACTORY_H
#include "memory"
#include "InterfaceModeState.h"
#include "GameMode.h"
#include "PersonnalisationMode.h"

namespace view {
    class StateFactory {
    public:
        static InterfaceModeState *getModeInstance(Mode mode) {
            switch (mode) {
                case Mode::game:
                    if (gameMode == nullptr) {
                        gameMode = std::make_unique<GameMode>();
                    }
                    return gameMode.get();
                case Mode::personalisation:
                    if (personnalisationMode == nullptr) {
                        personnalisationMode = std::make_unique<PersonnalisationMode>();
                    }
                    return personnalisationMode.get();
            }
            return nullptr;
        }

    private:
        inline static std::unique_ptr<GameMode> gameMode;
        inline static std::unique_ptr<PersonnalisationMode> personnalisationMode = nullptr;
    };

}

#endif //CHESS_STATEFACTORY_H
