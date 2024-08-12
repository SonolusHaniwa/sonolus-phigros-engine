using namespace std;

class Buckets {
    public:

    int note = 0;
    int drag = 1;
    int hold = 2;
    int flick = 3;
}Buckets;

auto bucket = defineBuckets<class Buckets>({
    {
        sprites: {
			{
				id: Sprites.NormalNote,
				x: 0,
				y: 0,
				w: 4.0,
				h: 2.0,
				rotation: 0
			}
        },
		unit: UnitText.Millisecond
    },
    {
        sprites: {
			{
				id: Sprites.NormalDrag,
				x: 0,
				y: 0,
				w: 4.0,
				h: 2.0,
				rotation: 0
			}
        },
		unit: UnitText.Millisecond
    },
    {
        sprites: {
			{
				id: Sprites.NormalHoldHead,
				x: -1.95,
				y: 0,
				w: 2.0,
				h: 0.1,
				rotation: 270
			},
			{
				id: Sprites.NormalHoldBody,
				x: 0.04,
				y: 0,
				w: 2.0,
				h: 3.9,
				rotation: 270
			}
        },
		unit: UnitText.Millisecond
    },
    {
        sprites: {
			{
				id: Sprites.NormalFlick,
				x: 0,
				y: 0,
				w: 4.0,
				h: 2.0,
				rotation: 0
			}
        },
		unit: UnitText.Millisecond
    }
});