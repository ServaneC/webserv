/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPAddress.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:33:16 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/09 02:14:15 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IP_ADDRESS_STRUCT_H
# define IP_ADDRESS_STRUCT_H

// typedef struct s_IPAddress
// {
//     unsigned char   block1;
//     unsigned char   block2;
//     unsigned char   block3;
//     unsigned char   block4;
// }               IPA_struct_t;

typedef union u_IPAddress
{
    unsigned char   block[4];
    unsigned int    address;
}                       IPA_t;

#endif