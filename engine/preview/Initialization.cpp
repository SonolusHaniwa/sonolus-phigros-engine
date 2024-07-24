class Initialization: public Archetype {
    public:

    static constexpr const char* name = "Phigros Initialization";

    SonolusApi preprocess() {
    	FUNCBEGIN
        let menuWidth = 0.15 * ui.menuConfiguration.scale;
        let menuHeight = 0.15 * ui.menuConfiguration.scale;
        let menuX = screen.r - interfaceGap;
        let menuY = screen.t - interfaceGap;
     	ui.menu.set(menuX, menuY, 1, 1, menuWidth, menuHeight, 0, ui.menuConfiguration.alpha, true);

		canvas.set(Scroll.LeftToRight, screen.aspectRatio);
        return VOID;
    }

    SonolusApi render() {
        FUNCBEGIN
        var height = 0.2;
        var B = -1 * height / 2, T = height / 2;
        var L = B * tipRatio, R = T * tipRatio;
        Draw(Sprites.tip, L, B, L, T, R, T, R, B, 100000, 1);
        return VOID;
    }
};
