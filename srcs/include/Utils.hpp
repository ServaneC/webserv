/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 12:10:32 by schene            #+#    #+#             */
/*   Updated: 2021/07/13 21:34:41 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include "../../webserv.hpp"

std::string     ft_itoa_cpp(int n);
int             hexa_to_int(std::string hexa);
size_t          ft_atoul(std::string ulong);

template <typename T>
T *MyRealloc( T*& src, int oldSize, int newSize ) 
{
  T *dst = new T[newSize];

  if ( newSize < oldSize )
    oldSize = newSize;

  std::memcpy(dst, src, oldSize);

  // Free the old block and reuse the pointer
  delete[] src;
  src = dst;

  // Return the pointer for convenience
  return src;
}

#endif