% this test tests every single conductance

all_cond = cpplab.search([fileparts(fileparts(which('xolotl'))) '/c++/conductances']);

if ~exist('ok','var')
	ok = false(length(all_cond),1);
end

disp('Testing every conductance one by one...')

n_error = 0;

for i = 1:length(all_cond)

	if any(strfind(all_cond{i},'templates')) || ok(i)
		ok(i) = true;
		continue
	end

	fprintf(all_cond{i});

	x = xolotl;
	x.add('compartment','AB','A',0.0628);



	try

		x.AB.add(all_cond{i});

		x.integrate;

		fprintf('  [OK]\n')
		ok(i) = true;
	catch
		fprintf('  [FAILED]\n')
	end

end


assert(all(ok),'Some conductances failed')
