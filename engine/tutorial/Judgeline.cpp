SonolusApi drawJudgeline(let, let) {
    FUNCBEGIN
    let x = 0, y = stage.b + stage.h * 0.2;
    let l = stage.l, r = stage.r;
    let b = y - judgelineHeight / 2, t = y + judgelineHeight / 2;
    Draw(
        If(HasSkinSprite(Sprites.AllPerfectJudgeline), Sprites.AllPerfectJudgeline, Sprites.NormalJudgeline), 
        l, b, l, t, r, t, r, b, 1000, 1
    );
    return VOID;
}