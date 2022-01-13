/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzoltan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:40:34 by jzoltan           #+#    #+#             */
/*   Updated: 2022/01/13 13:39:27 by jzoltan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

typedef struct s_ {
	char		buff[4096];
	va_list		args;
	char		*it;
	char		*re;
	const char	*format_str;
	char		s[20];
	int			num;
	int			d;
	int			f;
	int			r;
	int			count;
	int			i;
	int			l;
}	t_params;

typedef struct s_vector2i {
	int	x;
	int	y;
}	t_vector2i;

typedef struct s_vector4i {
	int	x;
	int	y;
	int	z;
	int	w;
}	t_vector4i;

typedef size_t			t_;
typedef const char *	t_t;

static int	ft_s(int c, t_params *p, const char *s)
{
	if (c == 'c')
		*(--p->re) = va_arg(p->args, int);
	*(t_vector2i*)&p->i = (t_vector2i){!c * p->i, !c * p->l + (c == 'c')};
	s = (char *)((c == 'c') * (size_t)p->re + (size_t)s * (c != 'c'));
	if (!s || ((size_t)--s && 0))
		return (ft_s(4, p, (const char *)((t_)"(null)" * !!(p->f & 1) + (t_)
				"0x0" * !(p->f & 1) + (!c || p->d < 6) * ((p->f & 1) + 5))));
	while (c && s[p->l + 1] && (p->l < p->d))
		++p->l;
	while (c && !(p->f & 128) && (p->num-- > p->l))
		ft_s(0, p, (t_t)((t_)" " * !(p->f & 256) + (t_)"0" * !!(p->f & 256)));
	while (p->r > -1 && ++s && ((c && p->i < p->l && p->i++ < p->d) + !c * *s))
	{
		if (p->it == p->buff + 4096)
		{
			p->r = write(1, p->buff, 4096);
			p->count += p->r;
			p->it = p->buff;
		}
		*(p->it++) = *s;
	}
	while (c && !!(p->f & 128) && (p->num-- > p->l))
		ft_s(0, p, " ");
	return (1);
}

static int	ft__in(int len, int s, t_params *p, int t)
{
	const char	*x[7] = {"", " ", "+", "+", "0x", "0X", "-"};
	const int	i = (s > 0) * ((p->f & 96) >> 5) + (s < 0) * 6
		+ (!s && *p->re > '0') * ((p->f & 8) / 2 + ((p->f & 24) == 24));

	if ((p->f & 1024) || !(*p->re || (--p->re)))
	{
		p->num = p->d;
		p->d = t;
		while (t > len && !(p->f & 128))
			ft_s(--t & 0, p, " ");
		ft_s(0, p, x[i]);
		p->f ^= (p->f & 128);
		p->d = INT_MAX;
		ft_s(4, p, p->re);
		while (t-- > len)
			ft_s(0, p, " ");
		return (1);
	}
	p->num -= ((t_)p->s + 19 - (t_)p->re) * !(p->f & 384);
	ft_s(!(p->f & 384), p, x[i]);
	p->num -= !!(p->f & 384) * (!!i + ((p->f & 8) && i));
	ft_s(4, p, p->re);
	return (1);
}

static int	ft_printf_int(int c, t_params *p, int f, size_t n)
{
	static const int	d[2][16] = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 17, 18, 19,
		20, 21, 22}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 49, 50, 51, 52, 53, 54}};
	int					s;
	int					l;

	if (c == 'p')
		n = va_arg(p->args, size_t);
	else
		n = va_arg(p->args, unsigned int);
	s = !(p->f & 4) * (-2 * ((int)n < 0) + 1);
	p->re[-1] = '0';
	while (n && (f & 4))
	{
		*--p->re = d[!(f & 16)][(n % (10 + ((f & 3) == 2) * 6))] + 48;
		n /= (10 + ((f & 3) == 2) * 6);
	}
	while (n)
	{
		*--p->re = d[!(f & 16)][((int)n % (10 + ((f & 3) == 2) * 6)) * s] + 48;
		*(int *)&n /= (10 + ((f & 3) == 2) * 6);
	}
	l = ((int)((t_)p->s + 19 - (t_)p->re) > p->d) * ((t_)p->s + 19 - (t_)p->re);
	l = l + !l * p->d + (s < 0 || (f & 100) > 4) + (f & 8) / 4;
	return ((!*p->re && c == 'p' && ft_s(4, p, 0)) || ft__in(l, s, p, p->num));
}

static int	ft_printf_format(const char *s, t_params *p)
{
	static int const	cod[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 8, 0, 1, 0, 0,
		0, 0, 0, 64, 0, 128, 0, 0, 256, 512, 512, 512, 512, 512, 512, 512, 512,
		512, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0,
		0, 3, 0, 0, 0, 0, 0, 0, 14, 0, 0, 1, 0, 7, 0, 0, 6, 0, 0, 0, 0, 0, 0};

	p->f = 0;
	*(t_vector2i *)&p->num = (t_vector2i){0, 0};
	while (!(cod[(unsigned char)*++s & 127] & 515) && cod[(unsigned char)*s])
		p->f |= cod[(unsigned char)*s & 127];
	while (cod[(unsigned char)*s & 127] & 768)
		p->num = (*s++ - '0' + (p->num * 10));
	p->f |= ((*s == '.') * 1280);
	while ((p->f & 1024) && (cod[(unsigned char)*++s & 127] & 768))
		p->d = (*s - '0' + (p->d * 10));
	p->f |= cod[(unsigned char)*s & 127];
	p->d = p->d * !!(p->f & 1024) + INT_MAX * !(p->f & 1024);
	p->format_str = (const char *)s;
	p->f ^= ((p->f & 3) == 1 && (p->f & 256)) << 8;
	return ((*s == 's' && ft_s(*s, p, va_arg(p->args, char *))) || (*s == '%'
			&& ft_s(37, p, "%")) || ((p->f & 2) && ft_printf_int(*s, p,
				p->f, 0)) || ((p->f & 1) && ft_s(*s, p, 0)));
}

int	ft_printf(const char *str, ...)
{
	t_params	params;

	params.it = params.buff;
	params.s[19] = 0 * (size_t)--str;
	*(t_vector4i *)&params.r = (t_vector4i){0, 0, 0, 0};
	va_start(params.args, str);
	while (params.r > -1 && *++str)
	{
		params.re = params.s + 19;
		if (*str == '%' && ft_printf_format(str, &params))
			str = params.format_str;
		else
		{
			*--params.re = *str;
			ft_s(0, &params, params.re);
		}
	}
	va_end(params.args);
	if (params.r > -1)
		return (params.count + write(1, params.buff, params.it - params.buff));
	return (-1);
}
