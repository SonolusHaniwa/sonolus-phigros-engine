SonolusApi Initialization() {
    FUNCBEGIN
    IF (HasSkinSprite(Sprites.PauseButton)) {
        let menuL = stage.l + 0.005 * stage.w;
        let menuR = stage.l + 0.05 * stage.w;
        let menuT = stage.t + 1.0 / 1080.0 * stage.h;
        let menuB = stage.t - 0.095 * stage.h;
        let menuX = (menuR + menuL) / 2;
        let menuY = (menuT + menuB) / 2;
        let menuWidth = menuR - menuL;
        let menuHeight = menuT - menuB;
        ui.menu.set(menuX, menuY, 0.5, 0.5, menuWidth, menuHeight, 0, 0, true);
    } ELSE {
        let menuWidth = 0.15;
        let menuHeight = 0.15;
        let menuX = stage.l + interfaceGap;
        let menuY = stage.t - interfaceGap;
        ui.menu.set(menuX, menuY, 0, 1, menuWidth, menuHeight, 0, 1, true);
    } FI

    var previousWidth = 0.15 * ui.navigationConfiguration.scale;
    var previousHeight = 0.15 * ui.navigationConfiguration.scale;
    var previousX = stage.l + interfaceGap;
    var previousY = 0;
    ui.navigationPrevious.set(previousX, previousY, 0, 0.5, previousWidth, previousHeight, 0, ui.navigationConfiguration.alpha, false);
    var nextX = stage.r - interfaceGap;
	var nextY = 0;
    ui.navigationNext.set(nextX, nextY, 1, 0.5, previousWidth, previousHeight, 0, ui.navigationConfiguration.alpha, false);

    var instructionWidth = 1.2 * ui.instructionConfiguration.scale;
    var instructionHeight = 0.15 * ui.instructionConfiguration.scale;
    ui.instruction.set(0, 0, 0.5, 0.5, instructionWidth, instructionHeight, 0, ui.instructionConfiguration.alpha, false);
    
    tutorialStartTime = 0;
    tutorialNavigation = 0;
    return VOID;
}