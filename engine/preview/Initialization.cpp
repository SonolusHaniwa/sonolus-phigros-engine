class Initialization: public Archetype {
    public:

    string name = "Phigros Initialization";

    SonolusApi preprocess() {
        var menuWidth = 0.15 * configuration.menu.scale;
        var menuHeight = 0.15 * configuration.menu.scale;
        var menuX = screen.r - interfaceGap;
        var menuY = screen.t - interfaceGap;
        setUI(
            ui.menu,
            menuX, menuY, 
            1, 1, 
            menuWidth, menuHeight, 
            0, 
            configuration.menu.alpha, 
            true
        );

        scroll = CanvasScroll.LeftToRight;
        canvasSize = screen.aspectRatio;
    }

    SonolusApi render() {
        var height = 0.2;
        var B = -1 * height / 2, T = height / 2;
        var L = B * tipRatio, R = T * tipRatio;
        Draw(Sprites.Tip, { L, B }, { L, T }, { R, T }, { R, B }, 100000, 1);
        Draw(
            Sprites.Blocker, 
            { 3 * screen.l, screen.b }, 
            { 3 * screen.l, screen.t }, 
            { 3 * screen.r, screen.t }, 
            { 3 * screen.r, screen.b }, 
            -1, 
            0.5
        );
    }
};
