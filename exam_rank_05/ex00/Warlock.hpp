/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Warlock.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaaouam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 15:53:07 by alaaouam          #+#    #+#             */
/*   Updated: 2023/09/13 15:53:24 by alaaouam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WARLOCK_HPP
# define WARLOCK_HPP

# include <iostream>

class Warlock
{
	public:
		Warlock(const std::string& name, const std::string& title);
		~Warlock(void);

		const std::string& getName(void) const;
		const std::string& getTitle(void) const;
		void setTitle(const std::string& title);
		void introduce(void) const;
	private:
		Warlock(void);
		Warlock(const Warlock& other);
		Warlock& operator=(const Warlock& other);

		std::string _name;
		std::string _title;
};

#endif