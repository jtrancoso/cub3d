#include "mlx/mlx.h"
#include <stdlib.h>
#include <math.h>

#define mapwidth 24
#define mapheight 24
#define screenwidth 640
#define screenheight 480

typedef struct s_player
{
	float	x;
	float	y;
	float	dir_x;
	float	dir_y;
	float	plane_x;
	float	plane_y;
	float	time;
	float	old_time;

}				t_player;

typedef struct	s_map
{
	int		line_height;
	int		draw_start;
	int		draw_end;
}				t_map;

/*typedef struct	s_rgb
{
	int		r;
	int		g;
	int		b;
}				t_rgb;*/

typedef struct	s_ray
{
	float	camera_x;
	float	dir_x;
	float	dir_y;
	int		map_x;
	int		map_y;
	float	side_dist_x;
	float	side_dist_y;
	float	delta_dist_x;
	float	delta_dist_y;
	float	perpwalldist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;

}				t_ray;
typedef struct	s_data
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	void	*mlx;
	void	*win;
	unsigned int rgb;
	int 	blockx;
	int		blocky;
	int		blocklen;
	int		width;
	t_player	player;
	t_ray		ray;
	t_map		map;
	//t_rgb		rgb;
	
}				t_data;

int		ft_close(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	exit(0);
	return(0);
}

int		ft_escape(int keycode, t_data *data)
{
	if (keycode == 53)
	{
		ft_close(data);
	}
	return(0);
}

int worldmap [mapwidth][mapheight] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

/*
** hola
** esto
** es
** estopa 
*/

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
	*(unsigned int*)dst = color;
}

void		ft_put_pixel(t_data *data, int x, int y, int color)
{
	data->addr[y * data->width + x] = color;
}


int main (int argc, char **argv)
{
	t_data data;
	data.width = 640;
	data.player.x = 22;
	data.player.y = 12;
	data.player.dir_x = -1;
	data.player.dir_y = 0;
	data.player.plane_x = 0;
	data.player.plane_y= 0.66;
	data.player.time = 0;
	data.player.old_time = 0;
	int x = 0;
	int color;
	
	data.mlx = mlx_init();
	data.win = mlx_new_window(data.mlx, screenwidth, screenheight, "raycaster");
	data.img = mlx_new_image(data.mlx, screenwidth, screenheight);
	data.addr = mlx_get_data_addr(data.img, &data.bpp, &data.line_len, &data.endian);
	while (x < screenwidth)
	{
		data.ray.camera_x = 2 * x / (float)screenwidth - 1;
		data.ray.dir_x = data.player.dir_x + data.player.plane_x * data.ray.camera_x;
		data.ray.dir_y = data.player.dir_y + data.player.plane_y * data.ray.camera_x;
		data.ray.map_x = (int)data.player.x;
		data.ray.map_y = (int)data.player.y;
		data.ray.delta_dist_x = data.ray.dir_y == 0 ? 1 : fabs(1 / data.ray.dir_x);
		data.ray.delta_dist_y = data.ray.dir_x == 0 ? 1 : fabs(1 / data.ray.dir_y);
		data.ray.hit = 0;

		if (data.ray.dir_x < 0)
		{
			data.ray.step_x = -1;
			data.ray.side_dist_x = (data.player.x - data.ray.map_x) * data.ray.delta_dist_x;
		}
		else
		{
			data.ray.step_x = 1;
			data.ray.side_dist_x = (data.ray.map_x + 1 - data.player.x) * data.ray.delta_dist_x;
		}
		if (data.ray.dir_y < 0)
		{
			data.ray.step_y = -1;
			data.ray.side_dist_y = (data.player.y - data.ray.map_y) * data.ray.delta_dist_y;
		}
		else
		{
			data.ray.step_y = 1;
			data.ray.side_dist_y = (data.ray.map_y + 1 - data.player.y) * data.ray.delta_dist_y;
		}
		while (data.ray.hit == 0)
		{
			if (data.ray.side_dist_x < data.ray.side_dist_y)
			{
				data.ray.side_dist_x += data.ray.delta_dist_x;
				data.ray.map_x += data.ray.step_x;
				data.ray.side = 0;
			}
			else
			{
				data.ray.side_dist_y += data.ray.delta_dist_y;
				data.ray.map_y += data.ray.step_y;
				data.ray.side = 1;
			}
			if (worldmap[data.ray.map_x][data.ray.map_y] > 0)
				data.ray.hit = 1;
		}
		if (data.ray.side == 0)
			data.ray.perpwalldist = (data.ray.map_x - data.player.x + (1 - data.ray.step_x) / 2) / data.ray.dir_x;
		else
			data.ray.perpwalldist = (data.ray.map_y - data.player.y + (1 - data.ray.step_y) / 2) / data.ray.dir_y;
		data.map.line_height = (int)(screenheight / data.ray.perpwalldist);
		data.map.draw_start = -data.map.line_height / 2 + screenheight / 2;
		if (data.map.draw_start < 0)
			data.map.draw_start = 0;
		data.map.draw_end = data.map.line_height / 2 + screenheight / 2;
		if (data.map.draw_end >= screenheight)
			data.map.draw_end = screenheight - 1;
		if (worldmap[data.ray.map_x][data.ray.map_y] == 1)
			color = 0xFF0000;
		else if (worldmap[data.ray.map_x][data.ray.map_y] == 2)
			color = 0x00FF00;
		else if (worldmap[data.ray.map_x][data.ray.map_y] == 3)
			color = 0x0000FF;
		else if (worldmap[data.ray.map_x][data.ray.map_y] == 4)
			color = 0xFFFFFF;
		else if (worldmap[data.ray.map_x][data.ray.map_y] == 5)
			color = 0x00FFFF;
		if (data.ray.side == 1)
			color /= 2;
		int y = data.map.draw_start;
		while (y < data.map.draw_end)
		{
			my_mlx_pixel_put(&data, x, y, color);
			y++;
			//printf("x: %d\n", x);
			//printf("y: %d\n", y);

		}
		x++;
	}
	mlx_key_hook(data.win, ft_escape, &data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_loop(data.mlx);
}