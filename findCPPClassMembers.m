% findCPPClassMembers.m
% reads a C++ file and finds the members of that class by
% figuring out the constructor for that class

function class_members = findCPPClassMembers(cppfilename)
	% check that it exists 
	assert(exist(cppfilename,'file') == 2,'C++ file not found.')

	class_name = pathEnd(cppfilename);
	lc = length(class_name);
	lines = lineRead(cppfilename);

	% find the lines where the class is declared
	constructor_lines = [];
	for i = 1:length(lines)
		if length(lines{i}) < lc
			continue
		end
		if strcmp(lines{i}(1:lc),class_name)
			constructor_lines = [constructor_lines; i];
		end
	end

	if length(constructor_lines) > 1
		error('More than one constructor; not coded')
	end

	constructor_line = lines{constructor_lines};

	% figure out the input variables to the constructor 
	input_variables = {};
	input_types = {};

	a = strfind(constructor_line,'(');
	z = strfind(constructor_line(a+1:end),','); z = z(1);
	while length(z) > 0
		z = z(1);
		this_input = strtrim(constructor_line(a+1:a+z-1));
		space_loc = strfind(this_input,' ');
		assert(length(space_loc)==1,'Expected exactly one space in this input')
		input_types = [input_types; strtrim(this_input(1:space_loc))];
		input_variables = [input_variables; strtrim(this_input(space_loc:end))];
		a = a+z;
		z = strfind(constructor_line(a+1:end),','); 
	end

	% read the actual constructor and figure out the mapping from the input variables to something
	% that something is assumed to be members of this class. 
	constructor_start = [];
	constructor_stop = [];
	idx = constructor_lines;

	for i = idx:length(lines)
		this_line = strtrim(lines{i});
		if strcmp(this_line(1),'{')
			constructor_start = i;
			break
		end
	end

	for i = constructor_start:length(lines)
		this_line = strtrim(lines{i});
		if strcmp(this_line(1),'}')
			constructor_stop = i;
			break
		end
	end

	% find every one of the input variables in the constructor code
	member_variables = cell(length(input_variables),1);

	for i = 1:length(member_variables)
		for j = constructor_start:constructor_stop
			this_line = strtrim(lines{j});
			this_line = strrep(this_line,' ','');

			if any(strfind(this_line,['=' input_variables{i} ';']))
				this_member = this_line(1:strfind(this_line,'=')-1);
				member_variables{i} = this_member;
			end
		end
	end

	class_members = member_variables;

end