Blocked SonolusApi Initialization() {
    if (HasSkinSprite(Sprites.PauseButton)) {
        var menuL = stage.l + 0.005 * stage.w;
        var menuR = stage.l + 0.05 * stage.w;
        var menuT = stage.t + 1.0 / 1080.0 * stage.h;
        var menuB = stage.t - 0.095 * stage.h;
        var menuX = (menuR + menuL) / 2;
        var menuY = (menuT + menuB) / 2;
        var menuWidth = menuR - menuL;
        var menuHeight = menuT - menuB;
        setUI(
            ui.menu,
            menuX, menuY, 
            0.5, 0.5, 
            menuWidth, menuHeight, 
            0, 
            0, 
            true
        );
    } else {
        var menuWidth = 0.15;
        var menuHeight = 0.15;
        var menuX = stage.l + interfaceGap;
        var menuY = stage.t - interfaceGap;
        setUI(
            ui.menu,
            menuX, menuY, 
            0, 1, 
            menuWidth, menuHeight, 
            0, 
            1, 
            true
        );
    }

    var previousWidth = 0.15 * configuration.navigation.scale;
    var previousHeight = 0.15 * configuration.navigation.scale;
    var previousX = stage.l + interfaceGap;
    var previousY = 0;
    setUI(
        ui.navigationPrevious,
        previousX, previousY,
        0, 0.5, 
        previousWidth, previousHeight, 
        0, 
        configuration.navigation.alpha, 
        false
    );
    var nextX = stage.r - interfaceGap;
	var nextY = 0;
    setUI(
        ui.navigationNext,
        nextX, nextY, 
        1, 0.5, 
        previousWidth, previousHeight, 
        0, 
        configuration.navigation.alpha, 
        false
    );

    var instructionWidth = 1.2 * configuration.instruction.scale;
    var instructionHeight = 0.15 * configuration.instruction.scale;
    setUI(
        ui.instruction,
        0, 0, 
        0.5, 0.5, 
        instructionWidth, instructionHeight, 
        0, 
        configuration.instruction.alpha, 
        false
    );
    
    tutorialStartTime = 0;
    tutorialNavigation = 0;
}

defineTutorialPreprocess(Initialization);

double tutorialFallTime = 4.0;
double tutorialClickTime = 4.0;
double tutorialDisappearTime = 1.0;