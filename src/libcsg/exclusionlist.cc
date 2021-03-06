/* 
 * Copyright 2009-2011 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <algorithm>
#include <votca/csg/topology.h>
#include <votca/csg/exclusionlist.h>

namespace votca { namespace csg {

void ExclusionList::Clear(void)
{
    list< exclusion_t *>::iterator iter;
    
    for(iter=_exclusions.begin();iter!=_exclusions.end();++iter)
        delete *iter;    
    _exclusions.clear(); 
}
    
void ExclusionList::CreateExclusions(Topology *top) {
	InteractionContainer &ic = top->BondedInteractions();
	InteractionContainer::iterator ia;

	for (ia = ic.begin(); ia != ic.end(); ++ia) {
		int beads_in_int = (*ia)->BeadCount();
		list<Bead *> l;

		for (int ibead = 0; ibead < beads_in_int; ibead ++) {
			int ii = (*ia)->getBeadId(ibead);
			l.push_back(top->getBead(ii));
		}
		ExcludeList(l);
	}
}

bool ExclusionList::IsExcluded(Bead *bead1, Bead *bead2) {
    exclusion_t *excl;
    if(bead1->getMolecule() != bead2->getMolecule()) return false;
    if (bead2 < bead1) swap(bead1, bead2);
    if ((excl = GetExclusions(bead1))) {
        if(find(excl->_exclude.begin(), excl->_exclude.end(), bead2) 
                != excl->_exclude.end()) return true;        
    }
    return false;
}

std::ostream &operator<<(std::ostream &out, ExclusionList& exl)
{
    list<ExclusionList::exclusion_t*>::iterator ex;
    
    for(ex=exl._exclusions.begin();ex!=exl._exclusions.end();++ex) {
        list<Bead *>::iterator i;
        out << (int)((*ex)->_atom->getId()) + 1;
        for(i=(*ex)->_exclude.begin(); i!=(*ex)->_exclude.end(); ++i) {
            out << " " << ((*i)->getId()+1);
        }
        out << endl;
    }
    return out;
}

}}
