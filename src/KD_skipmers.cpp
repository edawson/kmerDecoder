#include "kmerDecoder.hpp"

void Skipmers::seq_to_kmers(std::string & seq, std::vector <kmer_row> & kmers){

    kmers.clear();
    kmers.reserve(seq.size());

    std::string ORF_SEQ = "";
    for(auto const & start : this->ORFs){

        for(unsigned long i = start; i < seq.size(); i+=this->n){
                ORF_SEQ.append(seq.substr(i, this->m));
        }

        for(unsigned long j = 0; j < ORF_SEQ.size() - this->k + 1; j++){
            kmer_row kmer;
            kmer.str = ORF_SEQ.substr(j,this->k);
            kmer.hash = this->hasher->hash(kmer.str);
            kmers.push_back(kmer);
        }
        
        ORF_SEQ.clear();
    }

}

void Skipmers::extractKmers()
{
    std::string ORF_SEQ = "";


    for (int seqCounter = 0; seqCounter < this->chunk_size && ((kseq_read(this->kseqObj)) >= 0); seqCounter++) {

        std::string seq = kseqObj->seq.s;
        std::string id = kseqObj->name.s;
        
        this->kmers[id].reserve(seq.size());

        for(auto const & start : this->ORFs){

            for(unsigned long i = start; i < seq.size(); i+=this->n){
                ORF_SEQ.append(seq.substr(i, this->m));
            }

            for(unsigned long j = 0; j < ORF_SEQ.size() - this->k + 1; j++){
                kmer_row kmer;
                kmer.str = ORF_SEQ.substr(j,this->k);
                kmer.hash = this->hasher->hash(kmer.str);
                this->kmers[id].push_back(kmer);
            }

            ORF_SEQ.clear();
        }
    }

    if ((unsigned int) this->kmers.size() != this->chunk_size) {
        this->FILE_END = true;
    }

}