/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:28:39 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 15:01:29 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 800
#define HEIGHT 600

int main()
{
    void *mlx;
    void *win;

    // MiniLibX を初期化
    mlx = mlx_init();
    if (!mlx)
        return (write(1, "Error\n", 6), 1);

    // ウィンドウの作成
    win = mlx_new_window(mlx, WIDTH, HEIGHT, "legend of slime");

    // イベントループ開始
    mlx_loop(mlx);

    return (0);
}
