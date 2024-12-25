/*
 * NOTICE
 *
 * This software is licensed under the GNU Affero General Public License (AGPL) version 3.0 or later,
 * as published by the Free Software Foundation.
 *
 * Copyright (C) 2024 Guy Gnole/NeoLemnis
 *
 * Summary of key license terms:
 * - You are free to use, modify, and distribute this software, provided that you comply with the terms of the AGPL.
 * - Any modifications, adaptations, or derivative works must also be licensed under the AGPL and the source code
 *   must be made publicly available.
 * - If you use this software in a network application (e.g., web services, SaaS), the source code of the modified
 *   software must be made available to all users of the service.
 *
 * Commercial use:
 * This software cannot be used for commercial purposes without prior written consent from the author.
 * For inquiries regarding commercial licensing, please contact [guy.gnole@guy-gnole.fr](mailto:guy.gnole@guy-gnole.fr).
 *
 * Disclaimer:
 * This software is provided "as is," without warranty of any kind, express or implied, including but not limited to
 * the warranties of merchantability, fitness for a particular purpose, and noninfringement. In no event shall the
 * author or copyright holder be liable for any claim, damages, or other liability, whether in an action of contract,
 * tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software.
 *
 * For more information about the AGPL license, visit: <https://www.gnu.org/licenses/agpl-3.0.html>.
 */

#include <iostream>

#include "Matrix.h"

/**
 * @brief Entry point of the program.
 *
 * This program demonstrates the use of the Matrix class template.
 *
 * @return int Returns 0 upon successful execution.
 */
auto main() -> int
{
    try {
        Matrix<2, 2, int> matrix1{{1, 0}, {0, 1}};
        matrix1 *= 2;
        std::cout << "Matrix 1:\n" << matrix1 << '\n';

        Matrix<2, 2, int> matrix2{{1, 2}, {3, 4}};
        std::cout << "Matrix 2:\n" << matrix2 << '\n';

        Matrix<2, 2, int> matrix3{};
        matrix3 = matrix1 * matrix2;
        std::cout << "Matrix 3 (Matrix 1 + Matrix 2):\n" << matrix3 << '\n';
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << '\n';
        return 1;
    }
    return 0;
}