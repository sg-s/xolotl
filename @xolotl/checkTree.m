%{

### checkTree

**Syntax**

```matlab
x.checkTree
```

**Description**

This method checks that objects in the xolotl tree make sense
and are contained by objects that are allowed to contain them

The following rules are enforced:

| Object |   Legal container | 
| ------- | --------------- |
| compartment | xolotl object |
| mechanism | any |
| conductance | compartment | 
| synapse | compartment | 

This method is called in xolotl.transpile() before 
transpiling takes place

!!! info "See Also"
    ->xolotl.transpile
    ->xolotl.compile

%}

function checkTree(self)



comps = self.find('compartment');
assert(max(cellfun(@(x) length(strfind(x,'.')),comps))==0,'At least one compartment was nested incorrectly in the object tree')

conds = self.find('conductance');
if ~isempty(conds)
	assert(min(cellfun(@(x) length(strfind(x,'.')),conds))==1,'At least one conductance was nested incorrectly in the object tree')
	assert(max(cellfun(@(x) length(strfind(x,'.')),conds))==1,'At least one conductance was nested incorrectly in the object tree')
end

syns = self.find('synapse');
if ~isempty(syns)
	assert(min(cellfun(@(x) length(strfind(x,'.')),syns))==1,'At least one synapse was nested incorrectly in the object tree')
	assert(max(cellfun(@(x) length(strfind(x,'.')),syns))==1,'At least one synapse was nested incorrectly in the object tree')
end