#ifndef GAMESELECTORMENUPANEL_H
#define GAMESELECTORMENUPANEL_H

#include "stdafx.h"
#include "MenuPanel.h"

class GameSelectorMenuPanel : public MenuPanel {
public:
    GameSelectorMenuPanel();
    virtual ~GameSelectorMenuPanel();

    virtual void init() override;
    virtual void update() override;
    
private:
    Gwen::Controls::Button* m_singlePlayerButton;
    Gwen::Controls::Button* m_multiplayerButton;
    Gwen::Controls::Button* m_backButton;

    void onSinglePlayerClicked(Gwen::Controls::Base* control);
    void onMultiplayerClicked(Gwen::Controls::Base* control);
    void onBackClicked(Gwen::Controls::Base* control);
};

#endif
