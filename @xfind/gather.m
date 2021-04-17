% collects all data logs into one giant matrix

function [parameters, data] = gather(self)

% figure out how many rows we expect
n_rows = length(self.SimFcn) + length(self.ParameterNames);

% find all files
allfiles = dir([self.hash '*.xfind']);



% figure out all the sizes for all the files
file_sizes = NaN(length(allfiles),1);
for i = 1:length(allfiles)
	f = fopen(fullfile(allfiles(i).folder,allfiles(i).name),'r');
	fseek(f,0,'eof');

	% dividing by 8 because a double is 8 bytes
	file_sizes(i) = ftell(f)/8;
	fclose(f);
end

data = NaN(sum(file_sizes),1);

% now read from every file and stick it into data
a = 1;

for i = 1:length(allfiles)
	f = fopen(fullfile(allfiles(i).folder,allfiles(i).name),'r');
	data(a:a+file_sizes(i)-1) = fread(f,'double');
	fclose(f);
	a = a + file_sizes(i);
	
end

data = reshape(data,n_rows,length(data)/n_rows);
parameters = data(1:length(self.ParameterNames),:);
data = data(length(self.ParameterNames)+1:end,:);