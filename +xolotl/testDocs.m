function testDocs(allfiles)

if nargin == 0
	docsdir = [fileparts(fileparts(which('xolotl'))) filesep 'docs' filesep 'how-to'];
	allfiles = filelib.getAll(docsdir);
end


for i = 1:length(allfiles)


	[~,~,ext] = fileparts(allfiles(i));
	if ~strcmp(ext,'.md')
		continue
	end


	disp('Testing:')
	disp(allfiles{i})


	clearvars -except docsdir allfiles i 
	close all hidden

	code_blocks = filelib.extractCodeBlocks(allfiles{i});


	if isempty(code_blocks)
		return
	end


	try

		for j = 1:length(code_blocks)
			filelib.write('test.m',code_blocks(j).lines)

			% echo the lines we attempt to run...
			for k = 1:length(code_blocks(j).lines)
				disp(code_blocks(j).lines{k})
			end

			run('test.m')
			delete('test.m')

		end


	catch err

		if any(strfind(err.message,"Global Optimization"))
			warning("Error with Global Optimization toolbox, ignoring...")
		else
			error(['Error while running ' allfiles{i}])
		end
	end

end


close all hidden