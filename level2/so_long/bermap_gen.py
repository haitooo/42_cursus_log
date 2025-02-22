import random
import argparse

def generate_map(width, height, wall_density=0.15, filename="map.ber"):
    if width < 5 or height < 5:
        print("❌ エラー: マップサイズは最低 5x5 以上にしてください。")
        return

    # マップの基本構造を作成（初期状態はすべて '0'）
    map_data = [['0' for _ in range(width)] for _ in range(height)]

    # マップの外周を壁 (`1`) で囲む
    for x in range(width):
        map_data[0][x] = '1'  # 上
        map_data[height - 1][x] = '1'  # 下
    for y in range(height):
        map_data[y][0] = '1'  # 左
        map_data[y][width - 1] = '1'  # 右

    # プレイヤー (`P`) の配置
    player_x, player_y = random.randint(1, width - 2), random.randint(1, height - 2)
    map_data[player_y][player_x] = 'P'

    # アイテム (`C`) の配置（最低 1つ）
    num_collectibles = random.randint(1, (width * height) // 10)
    for _ in range(num_collectibles):
        while True:
            x, y = random.randint(1, width - 2), random.randint(1, height - 2)
            if map_data[y][x] == '0':  # 空白の場所に設置
                map_data[y][x] = 'C'
                break

    # 出口 (`E`) の配置（最低 1つ）
    while True:
        exit_x, exit_y = random.randint(1, width - 2), random.randint(1, height - 2)
        if map_data[exit_y][exit_x] == '0':  # 空白の場所に設置
            map_data[exit_y][exit_x] = 'E'
            break

    # 内部にランダムな壁 (`1`) を配置
    num_walls = int((width - 2) * (height - 2) * wall_density)  # 指定した割合のマスを壁にする
    for _ in range(num_walls):
        while True:
            x, y = random.randint(1, width - 2), random.randint(1, height - 2)
            if map_data[y][x] == '0':  # 既に `P`, `C`, `E` がない空白の場所のみ
                map_data[y][x] = '1'
                break

    # マップを `.ber` ファイルに保存
    with open(filename, "w") as f:
        for row in map_data:
            f.write("".join(row) + "\n")

    print(f"✅ {filename} を生成しました！（{width}x{height}, 壁密度: {wall_density*100:.0f}%）")

# コマンドラインオプションの処理
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="so_long 用の .ber マップを生成")
    parser.add_argument("-w", "--width", type=int, default=10, help="マップの幅（最小 5）")
    parser.add_argument("-H", "--height", type=int, default=6, help="マップの高さ（最小 5）")  # `-h` の代わりに `-H` を使用
    parser.add_argument("-d", "--density", type=float, default=0.15, help="壁の割合（0.0 〜 1.0 の範囲、デフォルト 0.15）")
    parser.add_argument("-o", "--output", type=str, default="generated_map.ber", help="出力ファイル名")

    args = parser.parse_args()

    generate_map(args.width, args.height, args.density, args.output)
