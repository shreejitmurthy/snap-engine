#define SNAP_IMPLEMENTATION
#include "snap.h"

int main() {
    snp_window_args args = {
            .title = "Window",
            .width = 800,
            .height = 600,
            .clear_colour.hex = 0x334d4d
    };

    system_init(args);

    snp_texture player_spritesheet = snp_texture_init("../examples/player.png");
    snp_quad player_quad = (snp_quad){0, 0, 64, 64};
    snp_vec2 player_position = {400, 300};

    snp_camera camera = snp_camera_init();
    camera.zoom = 2.0f;
    snp_camera_setpos(&camera, player_position);

    snp_texture knight = snp_texture_init("../examples/knight.png");

    snp_texture animals = snp_texture_init("../examples/animals.png");
    snp_quad kangaroo_quad = (snp_quad){1, 387, 32, 32};

    snp_texture health_bar = snp_texture_init("../examples/healthbar.png");

    while (snp_gfx_window_open()) {
        snp_gfx_clear();

        if (snp_keyboard_down(SNPK_ESCAPE)) {
            snp_app_state.window_open = false;
        }

        if (snp_keyboard_down(SNPK_A)) {
            player_position.x -= 1.f;
        } else if (snp_keyboard_down(SNPK_D)) {
            player_position.x += 1.f;
        }

        if (snp_keyboard_down(SNPK_W)) {
            player_position.y -= 1.f;
        } else if (snp_keyboard_down(SNPK_S)) {
            player_position.y += 1.f;
        }

        snp_camera_setpos(&camera, player_position);

        snp_camera_attach(camera);


        snp_texture_draw((snp_texture_draw_args){
                .texture = player_spritesheet,
                .quad = player_quad,
                .position = player_position
        });

        snp_texture_draw((snp_texture_draw_args){
                .texture = knight,
                .position = {200, 300},
                .sx = 2, .sy = 2
        });

        snp_texture_draw((snp_texture_draw_args){
                .texture = animals,
                .quad = kangaroo_quad,
                .position = {500, 400},
                .sx = 2, .sy = 2
        });

        snp_camera_detach();

        snp_texture_draw((snp_texture_draw_args){
                .texture = health_bar,
                .position = {75, 25},
                .sx = 2, .sy = 2
        });

        snp_gfx_refresh();
    }

    snp_texture_delete(player_spritesheet);

    snp_gfx_destroy();
    return 0;
}
