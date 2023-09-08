#include "../inc/block_parser.hpp"

ConfigParser::TokenType	ConfigParser::tokenization(std::istream *file, std::string *token) {
	if (file->eof())
		return TOKEN_TYPE_EOF;
	if (file->good() == false) {
		error_line_ = -1;
		return TOKEN_TYPE_ERROR;
	}
	while (true) {
		char c = file->get();
		switch (c) {
			case '{' :
				*token = c;
				return TOKEN_TYPE_START_BLOCK;
			case '}' :
				*token = c;
				return TOKEN_TYPE_END_BLOCK;
			case '#' :
				while (true) {
					c = file->get();
					if (c == '\n') {
						file->unget();
						return TOKEN_TYPE_COMMENT;
					}
				}
			case ';' :
				*token = c;
				return TOKEN_TYPE_SEMICOLON;
			case '\n' :
				*token = c;
				return TOKEN_TYPE_NEWLINE;
			case EOF :
				return TOKEN_TYPE_EOF;
			case '\t' :
			case ' ' :
			case '\r' :
				continue ;
			default :
				while (true) {
					*token += c;
					c = file->get();
					if (is_char_true(c)) {
						file->unget();
						return TOKEN_TYPE_NORMAL;
					}
				}
			return TOKEN_TYPE_DEFAULT;
		}
	}
}

int	ConfigParser::block_token_check(std::string token) {
	if (token == "http") {
		if (check_.http_ == true) {
			set_errorbit(2);
			return -1;
		}
		check_.http_ = true;
	}
	if (token == "server") {
		if (check_.server_ == true) {
			set_errorbit(3);
			return -1;
		}
		check_.server_ = true;
	}
	if (token == "location") {
		if (check_.location_ == true) {
			set_errorbit(4);
			return -1;
		}
		check_.location_ = true;
	}
	return 0;
}

void	ConfigParser::push_back_line(std::string token, std::string *line) {
	if (check_.http_ == true && check_.server_ == false && check_.location_ == false) {
		save_line_.push_back(*line);
		(*line).clear();
	}
	else if (check_.http_ == true && check_.server_ == true) {
		v_server_.push_back(*line);
		(*line).clear();
	}
	else if (check_.http_ == false && check_.server_ == false && check_.location_ == false) {
		save_line_.push_back(*line);
		(*line).clear();
	}
}

void	ConfigParser::start_parsing(std::istream *file) {
	TokenType	token_type = TOKEN_TYPE_DEFAULT;
	TokenType	last_type = TOKEN_TYPE_DEFAULT;
	std::string	line;
	std::string	token_tmp;

	while (true) {
		std::string	token;
		token_type = tokenization(file, &token);

		if (token_type == TOKEN_TYPE_NORMAL) {
			if (last_type == TOKEN_TYPE_NEWLINE || last_type == TOKEN_TYPE_DEFAULT )
				if (block_token_check(token) == -1)
					break ;
			line += token;
            token_tmp = token;
			push_back_line(token, &line);
		}

		if (token_type == TOKEN_TYPE_ERROR || last_type == TOKEN_TYPE_ERROR) {
			if (error_line_ != -1)
				error_line_ = line_num_ + 1;
			break ;
		}

		if (token_type == TOKEN_TYPE_EOF)
			break ;

		if (token_type == TOKEN_TYPE_SEMICOLON) {
			if (last_type == TOKEN_TYPE_START_BLOCK || last_type == TOKEN_TYPE_END_BLOCK || last_type == TOKEN_TYPE_SEMICOLON) {
				last_type = set_errorbit(1);
				continue ;
			}
			line += token;
			push_back_line(token, &line);
		}

		if (token_type == TOKEN_TYPE_NEWLINE) {
			if (last_type == TOKEN_TYPE_NORMAL) {
                last_type = set_errorbit(1);
                continue ;
            }
			line_num_++;
		}

		if (token_type == TOKEN_TYPE_START_BLOCK) {
			if (last_type == TOKEN_TYPE_SEMICOLON) {
				last_type = set_errorbit(1);
				continue;
			}
			if (check_.http_ == true && token_tmp == "http") {
					line += token;
					last_type = token_type;
					push_back_line(token, &line);
					token_tmp.clear();
					continue ;
			}
			if (check_.server_ == true && token_tmp == "server") {
					line += token;
					last_type = token_type;
					push_back_line(token, &line);
					token_tmp.clear();
					continue ;
			}
            if (check_.location_ == true && token_tmp.front() == '/') {
                line += token;
                last_type = token_type;
                push_back_line(token, &line);
                token_tmp.clear();
                continue ;
            }
			last_type = set_errorbit(1);
			continue;
		}
		if (token_type == TOKEN_TYPE_END_BLOCK) {
			if ((last_type == TOKEN_TYPE_NORMAL || last_type == TOKEN_TYPE_NEWLINE)
				&& (check_.http_ == true && check_.server_ == false && check_.location_ == false)) {
					line += token;
					save_line_.push_back(line);
					line.clear();
					last_type = token_type;
					continue ;
			}
			else if ((last_type == TOKEN_TYPE_NORMAL || last_type == TOKEN_TYPE_NEWLINE)
				&& (check_.http_ == true && check_.server_ == true && check_.location_ == false)) {
					line += token;
					v_server_.push_back(line);
					server_.push_back(v_server_);
					line.clear();
					v_server_.clear();
					last_type = token_type;
					check_.server_ = false;
					continue ;
			}
            else if ((last_type == TOKEN_TYPE_NORMAL || last_type == TOKEN_TYPE_NEWLINE)
                     && (check_.http_ == true && check_.server_ == true && check_.location_ == true)) {
                line += token;
                v_server_.push_back(line);
                line.clear();
                last_type = token_type;
                check_.location_ = false;
                continue ;
            }
			last_type = set_errorbit(1);
			continue;
		}
		last_type = token_type;
	}
}
