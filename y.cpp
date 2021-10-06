/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2021, Aleksandr Lyapunov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <vector>

#include <Table.hpp>

size_t num_months;
double default_wage_rub;
double initial_bonus_rub;
double initial_grant_usd;
double usual_grant_usd;
double usd_rate;

#define IN(var) do { std::cout << #var << ':'; std::cin >> var; } while(0)

int main()
{
	IN(num_months);
	IN(default_wage_rub);
	IN(initial_bonus_rub);
	IN(initial_grant_usd);
	IN(usual_grant_usd);
	IN(usd_rate);
	std::cout << std::endl;

	if (num_months < 24) {
		std::cout << "num_months must be 24+\n";
		return -1;
	}

	std::vector<double> salary(num_months, default_wage_rub);

	// initial bonus
	salary[0] += initial_bonus_rub;

	// review bonus
	for (size_t i = 6; i < num_months; i += 6)
		salary[i] += default_wage_rub * 0.8;

	// initial grant
	salary[11] += initial_grant_usd / 4 * usd_rate;
	for (size_t i = 1; i <= 12 && 11 + i * 3 < num_months; i++)
		salary[11 + i * 3] += initial_grant_usd / 16 * usd_rate;

	// review grant
	for (size_t i = 5; i < num_months; i += 6)
		for (size_t j = 1; j <= 16 && i + 3 * j < num_months; j++)
			salary[i + 3 * j] += usual_grant_usd / 16 * usd_rate;

	std::cout << row("month", "salary", "total", "average", "this year") << std::endl;
	std::cout << separator<5>() << std::endl;
	double total = 0;
	double year_acum = 0;
	for (size_t i = 0; i < num_months; i++) {
		total += salary[i];
		year_acum += salary[i];
		double this_year = 0;
		if (i % 12 == 11)
			std::swap(this_year, year_acum);
		std::cout << row(i, salary[i], total, total / (i + 1), this_year) << std::endl;
	}
}
