#ifndef CHESS_STATEFACTORY_HPP
#define CHESS_STATEFACTORY_HPP
#include "memory"
#include "InterfaceModeState.hpp"
#include "GameMode.hpp"
#include "PersonnalisationMode.hpp"

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

#endif //CHESS_STATEFACTORY_HPP
