typedef enum AtackType {
    LINE,
    AREA,
    POINT
} AtackType;

typedef struct EnemyAtack{
    int x;
    int y;
    int damage;
    int hit_box_size;
    AtackType atack_type;
} EnemyAtack;