#ifndef BLOCK_PARSER_HPP
# define BLOCK_PARSER_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include <cstring>

class ConfigParser {
	private:
		int		line_num_;
		int		error_bit_;
		int		error_line_;



		enum TokenType {
			TOKEN_TYPE_DEFAULT = 0,
			TOKEN_TYPE_NORMAL = 1,
			TOKEN_TYPE_START_BLOCK = 2,
			TOKEN_TYPE_END_BLOCK = 3,
			TOKEN_TYPE_NEWLINE = 4,
			TOKEN_TYPE_COMMENT = 5,
			TOKEN_TYPE_SEMICOLON = 6,
			TOKEN_TYPE_EOF = 7,
			TOKEN_TYPE_ERROR = 8
		};

		typedef struct s_check {
			bool	http_;
			bool	server_;
			bool	location_;
            bool    location_method_;

		}	t_check;

		t_check		check_;

		bool		is_char_true(char c);
		TokenType	tokenization(std::istream *file, std::string *token);
		TokenType	set_errorbit(int errorcode);
		int			block_token_check(std::string token);
		void		push_back_line(std::string token, std::string *line);

	public:
     std::vector<std::string>				save_line_;
     std::vector<std::string>				v_server_;
    std::vector<std::vector<std::string> >	server_;
		void		start_parsing(std::istream *file);
		void		init_parserclass(void);
		int			get_errorbit(void);
		int			get_errorline(void);
		void		print_parse_error(std::string filename);
		std::vector<std::string>	get_directives(void);
		std::vector<std::string>	get_server_i(int i);
		std::vector<std::vector<std::string> >	get_server(void);
};

int		check_extension(std::string filename);
int		check_block(std::string filename, ConfigParser *parser);

#endif
